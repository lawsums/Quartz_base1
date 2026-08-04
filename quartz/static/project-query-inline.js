/**
 * project-query-inline.js — 与 project-query.js 逻辑相同，
 * 但数据来自页面内嵌的 <script id="projects-data" type="application/json">，
 * 跳过 fetch 请求，页面加载瞬间渲染。
 *
 * 支持的 data 属性：
 *   data-status="已看"       按状态筛选（已看/想看/在看）
 *   data-category="anime"    按类别筛选（anime/manga/game）
 *   data-sort="rating"       排序字段（rating/year/title/noteId）
 *   data-order="DESC"         排序方向（DESC 降序 / ASC 升序）
 */

(function () {
  "use strict"

  const QUERY_SELECTOR = "[data-project-query-inline]"

  function encodePath(path) {
    return path
  }

  function filterProjects(projects, dataset) {
    let result = projects
    if (dataset.status) {
      result = result.filter((p) => p.status === dataset.status)
    }
    if (dataset.category) {
      result = result.filter((p) => p.category === dataset.category)
    }
    return result
  }

  function sortProjects(projects, dataset) {
    const sortField = dataset.sort || "rating"
    const order = (dataset.order || "DESC").toUpperCase() === "ASC" ? 1 : -1

    return [...projects].sort((a, b) => {
      let valA, valB
      switch (sortField) {
        case "rating":
          valA = a.rating || 0
          valB = b.rating || 0
          break
        case "year":
          valA = parseInt(a.year) || 0
          valB = parseInt(b.year) || 0
          break
        case "title":
          return order * a.title.localeCompare(b.title, "zh")
        case "noteId":
          valA = a.noteId || ""
          valB = b.noteId || ""
          return order * valA.localeCompare(valB)
        default:
          valA = a.rating || 0
          valB = b.rating || 0
      }
      return (valA - valB) * order
    })
  }

  function renderCard(project) {
    const cover = (project.cover || "").replace(/^http:\/\//, "https://")
    const title = escapeHtml(project.title)
    const subtitle = escapeHtml(project.subtitle || "")
    const status = project.status || ""
    const rating = project.rating > 0 ? project.rating.toFixed(1) : "—"
    const year = project.year || ""
    const category = project.category || ""
    const path = encodePath(project.path || "#")
    const statusClass = status ? `status-${status}` : ""

    let categoryLabel = ""
    if (category === "manga") categoryLabel = "漫画"
    else if (category === "game") categoryLabel = "游戏"

    const platform = escapeHtml(project.platform || "")
    const genre = escapeHtml(project.genre || "")
    const playtime = escapeHtml(project.playtime || "")

    let gameMetaHtml = ""
    if (category === "game") {
      const metaParts = []
      if (platform) metaParts.push(`<span class="project-card-platform">${platform}</span>`)
      if (genre) metaParts.push(`<span class="project-card-genre">${genre}</span>`)
      if (playtime) metaParts.push(`<span class="project-card-playtime">${playtime}</span>`)
      if (metaParts.length > 0) {
        gameMetaHtml = `<div class="project-card-game-meta">${metaParts.join("")}</div>`
      }
    }

    return `<a href="${path}" class="project-card ${statusClass}" data-category="${category}">
      <div class="project-card-cover">
        ${cover ? `<img src="${cover}" alt="${title}" loading="lazy" decoding="async" onerror="this.style.display='none';this.nextElementSibling.style.display='flex'" /><div class="project-card-no-cover" style="display:none">${title}</div>` : `<div class="project-card-no-cover">${title}</div>`}
        ${status ? `<span class="project-card-badge">${status}</span>` : ""}
        ${categoryLabel ? `<span class="project-card-cat">${categoryLabel}</span>` : ""}
      </div>
      <div class="project-card-info">
        <h3 class="project-card-title">${title}</h3>
        ${subtitle ? `<span class="project-card-subtitle">${subtitle}</span>` : ""}
        <div class="project-card-meta">
          <span class="project-card-rating">★ ${rating}</span>
          ${year ? `<span class="project-card-year">${year}</span>` : ""}
        </div>
        ${gameMetaHtml}
      </div>
    </a>`
  }

  function escapeHtml(str) {
    if (!str) return ""
    return str
      .replace(/&/g, "&amp;")
      .replace(/</g, "&lt;")
      .replace(/>/g, "&gt;")
      .replace(/"/g, "&quot;")
      .replace(/'/g, "&#039;")
  }

  function renderAll(projects) {
    const containers = document.querySelectorAll(QUERY_SELECTOR)
    if (containers.length === 0) return

    containers.forEach((container) => {
      const dataset = container.dataset
      let filtered = filterProjects(projects, dataset)
      filtered = sortProjects(filtered, dataset)

      if (filtered.length === 0) {
        container.innerHTML = `<p class="project-empty">暂无内容</p>`
        return
      }

      const cardsHtml = filtered.map(renderCard).join("")
      container.innerHTML = `<div class="project-grid">${cardsHtml}</div>`
    })
  }

  function init() {
    const scriptTag = document.getElementById("projects-data")
    if (!scriptTag) {
      console.error("[project-query-inline] 找不到 #projects-data 脚本标签")
      return
    }
    try {
      const data = JSON.parse(scriptTag.textContent)
      renderAll(data)
    } catch (err) {
      console.error("[project-query-inline] JSON 解析失败:", err)
    }
  }

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", init)
  } else {
    init()
  }
})()
