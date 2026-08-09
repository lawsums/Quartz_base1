#!/usr/bin/env node

/**
 * generate-anime-pages.js — 读取 projects-index.json，
 * 将番剧数据内嵌到番剧库.md 中，实现零 fetch 请求的瞬时渲染。
 *
 * 用法：node scripts/generate-anime-pages.js
 * 放在 generate-projects.js 和 cache-covers.js 之后运行
 */

import { readFileSync, writeFileSync, existsSync } from "fs"
import { join, dirname } from "path"
import { fileURLToPath } from "url"

const __dirname = dirname(fileURLToPath(import.meta.url))
const JSON_FILE = join(__dirname, "..", "quartz", "static", "projects-index.json")
const CONTENT_DIR = join(__dirname, "..", "content")
const TEMPLATE_FILE = join(CONTENT_DIR, "番剧库.md")

// ==================== 主逻辑 ====================

function generate() {
  if (!existsSync(JSON_FILE)) {
    console.error("[generate-anime-pages] JSON 文件不存在，先运行 generate-projects.js")
    process.exit(1)
  }
  if (!existsSync(TEMPLATE_FILE)) {
    console.error("[generate-anime-pages] 番剧库.md 模板不存在")
    process.exit(1)
  }

  const allProjects = JSON.parse(readFileSync(JSON_FILE, "utf-8"))
  const anime = allProjects.filter((p) => p.category === "anime")

  // 只保留前端需要的字段，减小页面体积
  const animeData = anime.map((p) => ({
    title: p.title,
    subtitle: p.subtitle,
    cover: p.cover,
    status: p.status,
    rating: p.rating,
    year: p.year,
    category: p.category,
    path: p.path,
    episodes: p.episodes,
    studio: p.studio,
  }))

  const jsonStr = JSON.stringify(animeData)
  let template = readFileSync(TEMPLATE_FILE, "utf-8")

  // 始终替换 <script id="projects-data"> 内的 JSON，不依赖 PLACEHOLDER_JSON 占位符
  // 避免"上次已替换→下次匹配不到占位符→静默跳过→数据永远不更新"的 bug
  const scriptBlockRegex = /<script id="projects-data" type="application\/json">[\s\S]*?<\/script>/
  const replacement = `<script id="projects-data" type="application/json">\n${jsonStr}\n</script>`

  if (scriptBlockRegex.test(template)) {
    const oldData = template.match(scriptBlockRegex)[0]
    template = template.replace(oldData, replacement)
    console.log(`[generate-anime-pages] 已替换 <script data> 块`)
  } else if (template.includes("PLACEHOLDER_JSON")) {
    // 向后兼容：如果模板还是旧的 PLACEHOLDER 格式
    template = template.replace("PLACEHOLDER_JSON", jsonStr)
    console.log(`[generate-anime-pages] 已替换 PLACEHOLDER_JSON`)
  } else {
    console.error(
      "[generate-anime-pages] 番剧库.md 中未找到 <script id=\"projects-data\"> 或 PLACEHOLDER_JSON，" +
      "请确保模板包含正确的占位标记"
    )
  }

  writeFileSync(TEMPLATE_FILE, template, "utf-8")

  const statusCounts = {}
  for (const a of anime) {
    statusCounts[a.status || "未知"] = (statusCounts[a.status || "未知"] || 0) + 1
  }

  console.log(`[generate-anime-pages] anime: ${anime.length} 条, JSON: ${(Buffer.byteLength(jsonStr) / 1024).toFixed(1)}KB`)
  console.log(`  状态分布: ${JSON.stringify(statusCounts)}`)
}

generate()
