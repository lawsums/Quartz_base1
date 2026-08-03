#!/usr/bin/env node

/**
 * generate-projects.js — 扫描 content/projects/ 下所有 .md 笔记，
 * 提取 frontmatter 元数据，清洗后输出为 quartz/static/projects-index.json
 *
 * 数据来源：Obsidian Bangumi 插件生成的番剧/漫画卡片
 * 用法：  node scripts/generate-projects.js
 */

import { readdirSync, readFileSync, writeFileSync, mkdirSync, existsSync } from "fs"
import { join, dirname, relative } from "path"
import { fileURLToPath } from "url"

const __dirname = dirname(fileURLToPath(import.meta.url))
const CONTENT_DIR = join(__dirname, "..", "content", "projects")
const OUTPUT_DIR = join(__dirname, "..", "quartz", "static")
const OUTPUT_FILE = join(OUTPUT_DIR, "projects-index.json")

// ==================== 工具函数 ====================

/**
 * 递归获取目录下所有 .md 文件
 */
function findMarkdownFiles(dir) {
  if (!existsSync(dir)) return []
  const results = []
  for (const entry of readdirSync(dir, { withFileTypes: true })) {
    const fullPath = join(dir, entry.name)
    if (entry.isDirectory()) {
      results.push(...findMarkdownFiles(fullPath))
    } else if (entry.name.endsWith(".md")) {
      results.push(fullPath)
    }
  }
  return results
}

/**
 * 从文件内容中提取 frontmatter（第一对 --- 之间的内容）
 * 返回 key-value 对象
 */
function parseFrontmatter(content) {
  const fmMatch = content.match(/^---\r?\n([\s\S]*?)\r?\n---/)
  if (!fmMatch) return null
  const fmText = fmMatch[1]
  const result = {}
  for (const line of fmText.split(/\r?\n/)) {
    const colonIdx = line.indexOf(":")
    if (colonIdx === -1) continue
    const key = line.slice(0, colonIdx).trim()
    let value = line.slice(colonIdx + 1).trim()
    // 去除首尾引号
    if (
      (value.startsWith('"') && value.endsWith('"')) ||
      (value.startsWith("'") && value.endsWith("'"))
    ) {
      value = value.slice(1, -1)
    }
    result[key] = value
  }
  return result
}

/**
 * 从 "![](http://...)" 格式中提取 URL
 */
function extractCoverUrl(value) {
  if (!value) return ""
  const match = value.match(/https?:\/\/[^\s)]+/)
  if (!match) return ""
  // 强制使用 HTTPS，避免在 HTTPS 站点上被浏览器混合内容策略拦截
  return match[0].replace(/^http:\/\//, "https://")
}

/**
 * 去除字符串中的 emoji 字符
 */
function stripEmoji(value) {
  if (!value) return ""
  return value.replace(/\p{Extended_Pictographic}/gu, "").trim()
}

/**
 * 清理数值字段（去除前导空格、引号）
 */
function cleanValue(value) {
  if (!value) return ""
  return value.trim()
}

/**
 * 将逗号分隔的标签拆分为数组
 */
function splitTags(value) {
  if (!value) return []
  return value
    .split(",")
    .map((t) => t.trim())
    .filter(Boolean)
}

/**
 * Quartz slugify 规则（与 @quartz-community/utils 的 slugifyPath 一致）
 * 每个路径段：空格→-、&→-and-、%→-percent、?→删除、#→删除、转小写
 */
function slugifyPath(s) {
  return s
    .split("/")
    .map((segment) =>
      segment
        .replace(/\s/g, "-")
        .replace(/&/g, "-and-")
        .replace(/%/g, "-percent")
        .replace(/\?/g, "")
        .replace(/#/g, "")
        .toLowerCase(),
    )
    .join("/")
    .replace(/\/$/, "")
}

/**
 * 将文件路径转换为 Quartz 页面路径（与 Quartz 内部 slug 生成一致）
 * content/projects/anime/3月的狮子 第二季.md → /projects/anime/3月的狮子-第二季
 */
function toPagePath(filePath) {
  const relPath = relative(join(__dirname, "..", "content"), filePath)
  // 统一路径分隔符为 /
  const normalized = relPath.replace(/\\/g, "/")
  // 去掉 .md 后缀
  const withoutExt = normalized.replace(/\.md$/, "")
  // 应用 Quartz slugify 规则
  return "/" + slugifyPath(withoutExt)
}

// ==================== 主逻辑 ====================

function generate() {
  console.log("[generate-projects] 开始生成项目索引...")

  const files = findMarkdownFiles(CONTENT_DIR)
  console.log(`[generate-projects] 找到 ${files.length} 个 .md 文件`)

  const projects = []

  for (const filePath of files) {
    const content = readFileSync(filePath, "utf-8")
    const fm = parseFrontmatter(content)

    // 跳过没有 frontmatter 或没有"中文名"字段的文件（如展示页 index.md）
    if (!fm || !fm["中文名"]) {
      continue
    }

    // 从路径推断类别（anime/manga/game）
    const relPath = relative(CONTENT_DIR, filePath).replace(/\\/g, "/")
    const category = relPath.split("/")[0] || "unknown"

    // 提取集数/话数（anime 用"集数"，manga 用"话数"）
    const episodes = cleanValue(fm["集数"] || fm["话数"] || "")

    // 提取制作方（anime 用"动画公司"，manga 用"出版社"，game 用"开发商"）
    const studio = cleanValue(fm["动画公司"] || fm["出版社"] || fm["开发商"] || "")

    // 提取年份（anime/manga 用"开播年份"，game 用"发售年份"）
    const year = cleanValue(fm["开播年份"] || fm["发售年份"] || "")

    // 提取评分（番剧用"Bangumi评分"，游戏用"评分"）
    const ratingStr = cleanValue(fm["Bangumi评分"] || fm["评分"] || "")
    const rating = parseFloat(ratingStr) || 0

    // 提取副标题（番剧用"日文名"，游戏用"外文名"）
    const subtitle = cleanValue(fm["日文名"] || fm["外文名"] || "")

    // 提取状态（番剧用"观看状态"，游戏用"游玩状态"）
    const status = stripEmoji(fm["观看状态"] || fm["游玩状态"] || "")

    // 游戏特有字段
    const platform = cleanValue(fm["平台"] || "")
    const genre = cleanValue(fm["类型"] || "")
    const playtime = cleanValue(fm["游玩时长"] || "")

    const project = {
      title: cleanValue(fm["中文名"]),
      subtitle: subtitle,
      cover: extractCoverUrl(fm["封面"] || ""),
      status: status,
      rating: rating,
      tags: splitTags(fm["标签"] || ""),
      episodes: episodes,
      year: year,
      studio: studio,
      platform: platform,
      genre: genre,
      playtime: playtime,
      category: category,
      path: toPagePath(filePath),
      noteId: cleanValue(fm["笔记ID"] || ""),
    }

    projects.push(project)
  }

  // 按标题排序（稳定排序，方便调试）
  projects.sort((a, b) => a.title.localeCompare(b.title, "zh"))

  // 确保输出目录存在
  if (!existsSync(OUTPUT_DIR)) {
    mkdirSync(OUTPUT_DIR, { recursive: true })
  }

  // 写入 JSON
  writeFileSync(OUTPUT_FILE, JSON.stringify(projects, null, 2), "utf-8")

  // 统计信息
  const statusCount = {}
  for (const p of projects) {
    statusCount[p.status] = (statusCount[p.status] || 0) + 1
  }

  console.log(`[generate-projects] 生成完成：${projects.length} 个项目`)
  console.log(`[generate-projects] 状态分布：${JSON.stringify(statusCount)}`)
  console.log(`[generate-projects] 输出文件：${OUTPUT_FILE}`)
}

generate()
