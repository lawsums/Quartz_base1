#!/usr/bin/env node

/**
 * cache-covers.js — 读取 projects-index.json，下载所有远程封面图到本地，
 * 并将 JSON 中的 cover 字段替换为本地路径。
 *
 * 缓存目录：quartz/static/covers/
 * 文件命名：URL 的 MD5 哈希前16位 + 原始扩展名
 *
 * 已缓存的图片会跳过下载（增量模式）。
 * 下载失败的条目保留原始远程 URL 作为 fallback。
 *
 * 用法：  node scripts/cache-covers.js
 */

import { readFileSync, writeFileSync, existsSync, mkdirSync, readdirSync, statSync } from "fs"
import { join, dirname, extname } from "path"
import { fileURLToPath } from "url"
import { createHash } from "crypto"
import { execFile } from "child_process"
import { promisify } from "util"
const execFileAsync = promisify(execFile)

const __dirname = dirname(fileURLToPath(import.meta.url))
const STATIC_DIR = join(__dirname, "..", "quartz", "static")
const COVERS_DIR = join(STATIC_DIR, "covers")
const JSON_FILE = join(STATIC_DIR, "projects-index.json")

/**
 * 用 curl 下载图片（curl 自动走系统代理，Node.js fetch 不走）
 * 返回 true/false 表示是否成功
 */
async function downloadImage(url, destPath, timeoutMs = 20000) {
  try {
    await execFileAsync("curl", [
      "-s", "-L",                    // 静默 + 跟随重定向
      "--max-time", "20",            // 总超时
      "--connect-timeout", "10",     // 连接超时
      "-o", destPath,                // 输出到文件
      "-H", "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36",
      "-H", "Accept: image/*,*/*;q=0.8",
      url,
    ], { timeout: timeoutMs })

    // 检查文件是否有效
    if (!existsSync(destPath)) return false
    const size = statSync(destPath).size
    if (size < 100) {
      console.error(`  文件过小 (${size} bytes): ${url}`)
      return false
    }
    return true
  } catch (err) {
    if (err.signal === "SIGTERM") {
      console.error(`  超时: ${url}`)
    } else {
      console.error(`  下载失败: ${url}`)
    }
    return false
  }
}

/** 从 URL 提取扩展名，默认 .jpg */
function getExtension(url) {
  try {
    const pathname = new URL(url).pathname
    const ext = extname(pathname).toLowerCase()
    if ([".jpg", ".jpeg", ".png", ".webp", ".gif", ".bmp", ".svg"].includes(ext)) {
      return ext
    }
  } catch {
    // ignore
  }
  return ".jpg"
}

/** 计算本地路径 */
function getLocalPath(url) {
  const hash = createHash("md5").update(url).digest("hex").slice(0, 16)
  const ext = getExtension(url)
  return {
    filename: `${hash}${ext}`,
    webPath: `./static/covers/${hash}${ext}`,
    localPath: join(COVERS_DIR, `${hash}${ext}`),
  }
}

/** 并发执行，限制并发数 */
async function parallelMap(items, fn, concurrency = 5) {
  const results = new Array(items.length)
  let index = 0
  async function worker() {
    while (index < items.length) {
      const i = index++
      results[i] = await fn(items[i], i)
    }
  }
  await Promise.all(Array.from({ length: Math.min(concurrency, items.length) }, () => worker()))
  return results
}

async function cacheCovers() {
  console.log("[cache-covers] 开始缓存封面图片...")

  if (!existsSync(JSON_FILE)) {
    console.error(`[cache-covers] JSON 文件不存在: ${JSON_FILE}`)
    console.error("[cache-covers] 请先运行 node scripts/generate-projects.js")
    process.exit(1)
  }

  // 读取 JSON
  const data = JSON.parse(readFileSync(JSON_FILE, "utf-8"))

  // 创建缓存目录
  if (!existsSync(COVERS_DIR)) {
    mkdirSync(COVERS_DIR, { recursive: true })
    console.log(`[cache-covers] 创建缓存目录: ${COVERS_DIR}`)
  }

  // 筛选需要处理的条目
  const toProcess = data.filter((item) => item.cover && !item.cover.startsWith("./static/"))

  if (toProcess.length === 0) {
    // 检查已有的本地缓存是否仍然有效
    const alreadyLocal = data.filter((item) => item.cover && item.cover.startsWith("./static/"))
    console.log(`[cache-covers] 所有 ${alreadyLocal.length} 个封面已是本地路径，无需下载`)
    return
  }

  console.log(`[cache-covers] 需要下载: ${toProcess.length} 个，并发: 5`)

  // 扫描已存在的缓存文件（用于跳过）
  const existingFiles = new Set(readdirSync(COVERS_DIR))

  let downloaded = 0
  let skipped = 0
  let failed = 0

  await parallelMap(toProcess, async (item) => {
    const { filename, webPath, localPath } = getLocalPath(item.cover)

    // 已缓存则跳过下载
    if (existingFiles.has(filename) && statSync(localPath).size > 100) {
      item.cover = webPath
      skipped++
      return
    }

    // 下载
    const success = await downloadImage(item.cover, localPath)
    if (success) {
      const sizeKB = (statSync(localPath).size / 1024).toFixed(1)
      item.cover = webPath
      downloaded++
      console.log(`  [OK] ${item.title} (${sizeKB}KB)`)
    } else {
      // 下载失败，保留原始远程 URL
      failed++
    }
  })

  // 写回 JSON
  writeFileSync(JSON_FILE, JSON.stringify(data, null, 2), "utf-8")

  // 统计
  const totalLocal = data.filter((d) => d.cover && d.cover.startsWith("./static/")).length
  const totalRemote = data.filter((d) => d.cover && !d.cover.startsWith("./static/")).length

  console.log(`[cache-covers] 完成: ${downloaded} 新下载, ${skipped} 已缓存跳过, ${failed} 失败`)
  console.log(`[cache-covers] 当前状态: ${totalLocal} 本地, ${totalRemote} 远程`)
  if (failed > 0) {
    console.log(`[cache-covers] ${failed} 个图片下载失败，保留远程 URL 作为 fallback`)
  }
}

cacheCovers().catch((err) => {
  console.error("[cache-covers] 致命错误:", err)
  process.exit(1)
})
