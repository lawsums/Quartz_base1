#!/usr/bin/env node

/**
 * fetch-game.js — 交互式游戏卡片生成器
 * 使用 RAWG API 搜索游戏，自动生成 Obsidian 笔记（frontmatter + 正文）
 *
 * 用法：
 *   node scripts/fetch-game.js "Elden Ring"    （直接搜索）
 *   node scripts/fetch-game.js                 （交互式输入）
 *
 * 配置 API Key（二选一）：
 *   方式一：创建 scripts/rawg-config.json，内容 {"apiKey": "你的key"}
 *   方式二：设置环境变量 RAWG_API_KEY
 *
 * 获取 API Key：https://rawg.io/apidocs （免费注册，2分钟搞定）
 */

import https from "https"
import http from "http"
import tls from "tls"
import { execSync } from "child_process"
import { readFileSync, writeFileSync, existsSync, mkdirSync } from "fs"
import { join, dirname } from "path"
import { fileURLToPath } from "url"
import readline from "readline"

const __dirname = dirname(fileURLToPath(import.meta.url))

// ==================== 配置 ====================

const CONFIG_FILE = join(__dirname, "rawg-config.json")
const OUTPUT_DIR = "E:/Documents/Obsidian_/Cpp/Projects/game"
const RAWG_BASE = "https://api.rawg.io/api"

// 游玩状态映射
const STATUS_OPTIONS = [
  { key: "1", label: "已通关", emoji: "🎮" },
  { key: "2", label: "在玩", emoji: "🕹️" },
  { key: "3", label: "想玩", emoji: "💭" },
  { key: "4", label: "搁置", emoji: "⏸️" },
]

// ==================== 代理支持 ====================

/**
 * 从 Windows 注册表读取系统代理设置
 */
function getWindowsProxy() {
  if (process.platform !== "win32") return null
  try {
    const regPath = "HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"

    // 检查代理是否启用
    const enableOut = execSync(`reg query "${regPath}" /v ProxyEnable`, {
      timeout: 3000,
      encoding: "utf-8",
    })
    if (!/0x1/i.test(enableOut)) return null

    // 读取代理服务器地址
    const serverOut = execSync(`reg query "${regPath}" /v ProxyServer`, {
      timeout: 3000,
      encoding: "utf-8",
    })
    const match = serverOut.match(/ProxyServer\s+REG_SZ\s+(.+)/)
    if (!match) return null

    const serverStr = match[1].trim()

    // ProxyServer 格式可能为 "host:port" 或 "http=host:port;https=host:port"
    if (serverStr.includes("=")) {
      const parts = serverStr.split(";")
      for (const part of parts) {
        const [proto, addr] = part.split("=")
        if (proto.trim() === "https" || proto.trim() === "http") {
          const url = new URL(`http://${addr.trim()}`)
          return { host: url.hostname, port: parseInt(url.port) || 80 }
        }
      }
      return null
    } else {
      const url = new URL(`http://${serverStr}`)
      return { host: url.hostname, port: parseInt(url.port) || 80 }
    }
  } catch {
    return null
  }
}

/**
 * 检测系统代理（环境变量 > Windows 注册表）
 */
function getProxy() {
  // 1. 检查环境变量
  const proxyStr =
    process.env.HTTPS_PROXY || process.env.https_proxy ||
    process.env.HTTP_PROXY || process.env.http_proxy ||
    process.env.ALL_PROXY || process.env.all_proxy
  if (proxyStr) {
    try {
      const url = new URL(proxyStr)
      return { host: url.hostname, port: parseInt(url.port) || 80 }
    } catch {}
  }

  // 2. Windows: 检查系统代理（注册表）
  return getWindowsProxy()
}

const PROXY = getProxy()

/**
 * 解析原始 HTTP 响应，处理 chunked 编码
 */
function parseHttpResponse(rawData) {
  const headerEnd = rawData.indexOf("\r\n\r\n")
  if (headerEnd < 0) throw new Error("无效的 HTTP 响应")

  const headerStr = rawData.slice(0, headerEnd)
  let body = rawData.slice(headerEnd + 4)

  // 检查是否 chunked 编码
  const headers = {}
  headerStr.split("\r\n").slice(1).forEach((line) => {
    const idx = line.indexOf(":")
    if (idx > 0) {
      headers[line.slice(0, idx).trim().toLowerCase()] =
        line.slice(idx + 1).trim().toLowerCase()
    }
  })

  if (headers["transfer-encoding"] === "chunked") {
    let result = ""
    let pos = 0
    while (pos < body.length) {
      const lineEnd = body.indexOf("\r\n", pos)
      if (lineEnd < 0) break
      const chunkSize = parseInt(body.slice(pos, lineEnd), 16)
      if (isNaN(chunkSize) || chunkSize === 0) break
      result += body.slice(lineEnd + 2, lineEnd + 2 + chunkSize)
      pos = lineEnd + 2 + chunkSize + 2
    }
    body = result
  }

  return body
}

// ==================== 工具函数 ====================

/**
 * 获取 API Key（环境变量 > 配置文件）
 */
function getApiKey() {
  if (process.env.RAWG_API_KEY) return process.env.RAWG_API_KEY

  if (existsSync(CONFIG_FILE)) {
    try {
      const config = JSON.parse(readFileSync(CONFIG_FILE, "utf-8"))
      if (config.apiKey) return config.apiKey
    } catch (e) {
      // 配置文件解析失败，忽略
    }
  }
  return null
}

/**
 * HTTPS GET 请求（自动检测代理），返回 JSON
 * - 有代理：CONNECT 隧道 + TLS
 * - 无代理：直连
 */
function httpsGet(url) {
  return new Promise((resolve, reject) => {
    let timer = null
    const cleanup = () => { if (timer) { clearTimeout(timer); timer = null } }
    const fail = (err) => { cleanup(); reject(err) }

    timer = setTimeout(() => fail(new Error("请求超时 (15s)")), 15000)

    if (PROXY) {
      // 通过代理建立 CONNECT 隧道
      const targetUrl = new URL(url)
      const connectReq = http.request({
        host: PROXY.host,
        port: PROXY.port,
        method: "CONNECT",
        path: `${targetUrl.hostname}:443`,
        headers: { Host: `${targetUrl.hostname}:443` },
      })

      connectReq.on("connect", (proxyRes, socket) => {
        if (proxyRes.statusCode !== 200) {
          fail(new Error(`代理连接失败: ${proxyRes.statusCode}`))
          return
        }

        // 在隧道上建立 TLS 连接
        const tlsSocket = tls.connect({
          socket: socket,
          servername: targetUrl.hostname,
        }, () => {
          const path = targetUrl.pathname + targetUrl.search
          tlsSocket.write(
            `GET ${path} HTTP/1.1\r\n` +
            `Host: ${targetUrl.hostname}\r\n` +
            `Connection: close\r\n` +
            `Accept: application/json\r\n\r\n`
          )

          let rawData = ""
          tlsSocket.on("data", (chunk) => (rawData += chunk.toString()))
          tlsSocket.on("end", () => {
            cleanup()
            try {
              const body = parseHttpResponse(rawData)
              const json = JSON.parse(body)
              if (json.detail) {
                reject(new Error(json.detail))
              } else {
                resolve(json)
              }
            } catch (e) {
              reject(new Error(`JSON 解析失败: ${e.message}`))
            }
          })
          tlsSocket.on("error", fail)
        })
        tlsSocket.on("error", fail)
      })

      connectReq.on("error", fail)
      connectReq.end()
    } else {
      // 直连（无代理）
      const req = https.get(url, { timeout: 15000 }, (res) => {
        let data = ""
        res.on("data", (chunk) => (data += chunk))
        res.on("end", () => {
          cleanup()
          try {
            const json = JSON.parse(data)
            if (json.detail) {
              reject(new Error(json.detail))
            } else {
              resolve(json)
            }
          } catch (e) {
            reject(new Error(`JSON 解析失败: ${e.message}`))
          }
        })
        res.on("error", fail)
      })
      req.on("error", fail)
      req.on("timeout", () => fail(new Error("请求超时 (15s)")))
    }
  })
}

/**
 * readline question 封装为 Promise
 */
function prompt(rl, question) {
  return new Promise((resolve) => {
    rl.question(question, (answer) => resolve(answer.trim()))
  })
}

/**
 * 搜索游戏
 */
async function searchGames(apiKey, query) {
  const url = `${RAWG_BASE}/games?key=${apiKey}&search=${encodeURIComponent(query)}&page_size=10`
  const data = await httpsGet(url)
  return data.results || []
}

/**
 * 获取游戏详情
 */
async function getGameDetails(apiKey, slug) {
  const url = `${RAWG_BASE}/games/${slug}?key=${apiKey}`
  return httpsGet(url)
}

/**
 * 从名称生成安全的文件名
 */
function safeFileName(name) {
  return name
    .replace(/[\\/:*?"<>|]/g, "_")
    .replace(/\s+/g, " ")
    .trim()
}

/**
 * 清理 HTML 标签，转为纯文本
 */
function stripHtml(html) {
  if (!html) return ""
  return html
    .replace(/<br\s*\/?>/gi, "\n")
    .replace(/<\/p>/gi, "\n\n")
    .replace(/<[^>]+>/g, "")
    .replace(/&amp;/g, "&")
    .replace(/&lt;/g, "<")
    .replace(/&gt;/g, ">")
    .replace(/&quot;/g, '"')
    .replace(/&#39;/g, "'")
    .replace(/\n{3,}/g, "\n\n")
    .trim()
}

// ==================== 生成 Markdown ====================

/**
 * 生成 frontmatter
 */
function generateFrontmatter(details, chineseName, status, rating) {
  const cover = details.background_image
    ? `![](${details.background_image})`
    : ""

  const platforms = (details.platforms || [])
    .map((p) => p.platform?.name)
    .filter(Boolean)
    .join(", ")

  const genres = (details.genres || [])
    .map((g) => g.name)
    .filter(Boolean)
    .join(", ")

  const developers = (details.developers || [])
    .map((d) => d.name)
    .filter(Boolean)
    .join(", ")

  const year = details.released ? details.released.slice(0, 4) : ""

  const tags = (details.tags || [])
    .slice(0, 10)
    .map((t) => t.name)
    .filter(Boolean)
    .join(", ")

  const playtime = details.playtime ? `${details.playtime}h` : ""

  const lines = [
    "---",
    `中文名: "${chineseName || details.name}"`,
    `外文名: "${details.name}"`,
  ]

  if (cover) lines.push(`封面: "${cover}"`)
  lines.push(`游玩状态: ${status}`)
  if (rating) lines.push(`评分: "${rating}"`)
  if (platforms) lines.push(`平台: ${platforms}`)
  if (genres) lines.push(`类型: ${genres}`)
  if (developers) lines.push(`开发商: ${developers}`)
  if (year) lines.push(`发售年份: "${year}"`)
  if (playtime) lines.push(`游玩时长: "${playtime}"`)
  if (tags) lines.push(`标签: ${tags}`)
  lines.push("---")

  return lines.join("\n")
}

/**
 * 生成 Markdown 正文
 */
function generateBody(details) {
  const parts = []

  // 游戏描述
  if (details.description_raw) {
    const desc = stripHtml(details.description_raw)
    if (desc) {
      parts.push(desc)
    }
  }

  // 基础信息表
  parts.push("\n## 基本信息")
  if (details.released) parts.push(`- 发售日期: ${details.released}`)
  if (details.metacritic) parts.push(`- Metacritic: ${details.metacritic}`)
  if (details.rating) parts.push(`- RAWG 评分: ${details.rating}/5`)
  if (details.website) parts.push(`- 官网: ${details.website}`)
  if (details.esrb_rating?.name) parts.push(`- 分级: ${details.esrb_rating.name}`)

  const publishers = (details.publishers || [])
    .map((p) => p.name)
    .filter(Boolean)
    .join(", ")
  if (publishers) parts.push(`- 发行商: ${publishers}`)

  if (details.playtime) parts.push(`- 平均通关时长: ${details.playtime} 小时`)

  return parts.join("\n")
}

// ==================== 主流程 ====================

async function main() {
  console.log("========================================")
  console.log("   RAWG 游戏卡片生成器")
  console.log("========================================\n")

  if (PROXY) {
    console.log(`(代理已启用: ${PROXY.host}:${PROXY.port})\n`)
  } else {
    console.log("(直连模式，无代理)\n")
  }

  // 检查 API Key
  const apiKey = getApiKey()
  if (!apiKey) {
    console.log("[ERROR] 未找到 RAWG API Key!\n")
    console.log("请按以下步骤获取：")
    console.log("  1. 打开 https://rawg.io/apidocs")
    console.log("  2. 注册 rawg.io 账号")
    console.log("  3. 在 Get API Key 处获取你的 API Key\n")
    console.log("然后选择一种方式配置：")
    console.log(`  方式一：创建文件 ${CONFIG_FILE}`)
    console.log('         内容: {"apiKey": "你的key"}')
    console.log("  方式二：设置环境变量")
    console.log("         PowerShell: $env:RAWG_API_KEY='你的key'")
    console.log("         Bash:       export RAWG_API_KEY=你的key\n")
    process.exit(1)
  }

  // 获取搜索关键词
  let query = process.argv[2]
  if (!query) {
    const rl = readline.createInterface({
      input: process.stdin,
      output: process.stdout,
    })
    query = await prompt(rl, "输入游戏名称 (英文搜索效果更好): ")
    rl.close()
  }
  if (!query) {
    console.log("未输入游戏名称")
    process.exit(1)
  }

  // 搜索游戏
  console.log(`\n搜索 "${query}" ...`)
  let results
  try {
    results = await searchGames(apiKey, query)
  } catch (e) {
    console.log(`搜索失败: ${e.message}`)
    process.exit(1)
  }

  if (results.length === 0) {
    console.log("未找到相关游戏，请尝试其他关键词")
    process.exit(1)
  }

  // 显示搜索结果
  console.log(`\n找到 ${results.length} 个结果：\n`)
  results.forEach((game, i) => {
    const year = game.released ? game.released.slice(0, 4) : "----"
    const rating = game.rating ? ` ★${game.rating}` : ""
    const platforms = (game.platforms || [])
      .map((p) => p.platform?.name)
      .filter(Boolean)
      .slice(0, 4)
      .join(", ")
    console.log(`  [${i + 1}] ${game.name} (${year})${rating}`)
    if (platforms) console.log(`      平台: ${platforms}`)
  })

  // 交互式选择
  const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
  })

  const choice = await prompt(
    rl,
    `\n选择编号 (1-${results.length}, q 退出): `,
  )
  if (choice.toLowerCase() === "q" || choice === "quit") {
    console.log("已退出")
    rl.close()
    process.exit(0)
  }

  const idx = parseInt(choice) - 1
  if (isNaN(idx) || idx < 0 || idx >= results.length) {
    console.log("无效选择")
    rl.close()
    process.exit(1)
  }

  const selected = results[idx]
  console.log(`\n已选择: ${selected.name}`)

  // 获取游戏详情
  console.log("获取详情中...")
  let details
  try {
    details = await getGameDetails(apiKey, selected.slug)
  } catch (e) {
    console.log(`获取详情失败: ${e.message}`)
    console.log("将使用搜索结果中的数据")
    details = selected
  }

  // 显示详情摘要
  console.log("\n--- 游戏信息 ---")
  console.log(`名称:     ${details.name}`)
  console.log(`发售日:   ${details.released || "未知"}`)
  if (details.rating) console.log(`RAWG评分: ${details.rating}/5`)
  if (details.metacritic) console.log(`Metacritic: ${details.metacritic}`)

  const genres = (details.genres || []).map((g) => g.name).join(", ")
  if (genres) console.log(`类型:     ${genres}`)

  const platforms = (details.platforms || [])
    .map((p) => p.platform?.name)
    .join(", ")
  if (platforms) console.log(`平台:     ${platforms}`)

  const developers = (details.developers || [])
    .map((d) => d.name)
    .join(", ")
  if (developers) console.log(`开发商:   ${developers}`)

  if (details.playtime) console.log(`平均时长: ${details.playtime}h`)

  // 用户填写卡片信息
  console.log("\n--- 填写卡片信息 ---\n")

  const chineseName = await prompt(
    rl,
    `中文名 (回车使用原名 "${details.name}"): `,
  )

  console.log("\n游玩状态:")
  STATUS_OPTIONS.forEach((s) => {
    console.log(`  ${s.key}. ${s.label} ${s.emoji}`)
  })
  const statusChoice = await prompt(rl, "选择 (1-4, 默认3想玩): ")
  const statusOpt =
    STATUS_OPTIONS.find((s) => s.key === statusChoice) || STATUS_OPTIONS[2]
  const status = `${statusOpt.label}${statusOpt.emoji}`

  const ratingInput = await prompt(
    rl,
    "你的评分 (0-10, 回车跳过): ",
  )
  let rating = ""
  if (ratingInput) {
    const parsed = parseFloat(ratingInput)
    if (!isNaN(parsed) && parsed >= 0 && parsed <= 10) {
      rating = parsed.toFixed(1)
    } else {
      console.log("评分无效，已跳过")
    }
  }

  const playtimeOverride = await prompt(
    rl,
    `游玩时长 (回车使用 RAWG 数据 ${details.playtime ? details.playtime + "h" : "无"}): `,
  )

  rl.close()

  // 生成 Markdown
  const finalChineseName = chineseName || details.name
  const frontmatter = generateFrontmatter(
    details,
    finalChineseName,
    status,
    rating,
  )
  const body = generateBody(details)

  // 如果用户自定义了游玩时长，覆盖 frontmatter 中的值
  let finalMarkdown = frontmatter + "\n\n" + body + "\n"
  if (playtimeOverride) {
    finalMarkdown = finalMarkdown.replace(
      /游玩时长: ".*?"/,
      `游玩时长: "${playtimeOverride}"`,
    )
  }

  // 保存文件
  const fileName = safeFileName(finalChineseName) + ".md"
  const outputPath = join(OUTPUT_DIR, fileName)

  // 确保目录存在
  if (!existsSync(OUTPUT_DIR)) {
    mkdirSync(OUTPUT_DIR, { recursive: true })
    console.log(`创建目录: ${OUTPUT_DIR}`)
  }

  // 检查文件是否已存在
  if (existsSync(outputPath)) {
    console.log(`\n⚠️ 文件已存在: ${outputPath}`)
    console.log("将覆盖...")
  }

  writeFileSync(outputPath, finalMarkdown, "utf-8")

  console.log(`\n✅ 已保存: ${outputPath}`)
  console.log(`\n文件名: ${fileName}`)
  console.log(`\n下一步:`)
  console.log(`  1. 在 Obsidian 中检查/编辑该笔记`)
  console.log(`  2. 运行同步: bash scripts/sync-obsidian.sh`)
  console.log(`  3. 或仅生成索引: node scripts/generate-projects.js`)
}

main().catch((err) => {
  console.error("运行出错:", err)
  process.exit(1)
})
