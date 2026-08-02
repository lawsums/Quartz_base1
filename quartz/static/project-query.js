/**
 * project-query.js — 浏览器端读取 projects-index.json，在页面中的
 * <div data-project-query> 占位元素内渲染番剧/漫画卡片。
 *
 * 支持的 data 属性：
 *   data-status="已看"       按状态筛选（已看/想看/在看）
 *   data-category="anime"    按类别筛选（anime/manga/game）
 *   data-sort="rating"       排序字段（rating/year/title/noteId）
 *   data-order="DESC"         排序方向（DESC 降序 / ASC 升序）
 *
 * 用法：在 Markdown 中写
 *   <div data-project-query data-status="已看" data-sort="rating" data-order="DESC"></div>
 */

(function () {
  "use strict"

  // 卡片占位符选择器
  const QUERY_SELECTOR = "[data-project-query]"
  // JSON 数据路径
  const JSON_URL = "/static/projects-index.json"

  /**
   * 获取 URL 友好的路径（处理空格和特殊字符）
   */
  function encodePath(path) {
    // Quartz 生成的 URL 用空格而非 %20，保持一致
    return path
  }

  /**
   * 根据 data 属性筛选数据
   */
  function filterProjects(projects, dataset) {
    let result = projects

    // 按状态筛选
    if (dataset.status) {
      result = result.filter((p) => p.status === dataset.status)
    }

    // 按类别筛选
    if (dataset.category) {
      result = result.filter((p) => p.category === dataset.category)
    }

    return result
  }

  /**
   * 排序数据
   */
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
          // noteId 是时间戳字符串，按字符串比较即可
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

  /**
   * 生成单张卡片的 HTML
   */
  function renderCard(project) {
    const cover = project.cover || ""
    const title = escapeHtml(project.title)
    const subtitle = escapeHtml(project.subtitle || "")
    const status = project.status || ""
    const rating = project.rating > 0 ? project.rating.toFixed(1) : "—"
    const year = project.year || ""
    const category = project.category || ""
    const path = encodePath(project.path || "#")

    // 状态 CSS 类名
    const statusClass = status ? `status-${status}` : ""

    // 类别标签
    let categoryLabel = ""
    if (category === "manga") categoryLabel = "漫画"
    else if (category === "game") categoryLabel = "游戏"

    // 游戏额外信息
    const platform = escapeHtml(project.platform || "")
    const genre = escapeHtml(project.genre || "")
    const playtime = escapeHtml(project.playtime || "")

    // 游戏卡片额外行
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
        ${cover ? `<img src="${cover}" alt="${title}" loading="lazy" />` : `<div class="project-card-no-cover">${title}</div>`}
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

  /**
   * HTML 转义
   */
  function escapeHtml(str) {
    if (!str) return ""
    return str
      .replace(/&/g, "&amp;")
      .replace(/</g, "&lt;")
      .replace(/>/g, "&gt;")
      .replace(/"/g, "&quot;")
      .replace(/'/g, "&#039;")
  }

  /**
   * 渲染所有占位容器
   */
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

  /**
   * 初始化：fetch JSON → 渲染
   */
  function init() {
    fetch(JSON_URL)
      .then((res) => {
        if (!res.ok) throw new Error(`HTTP ${res.status}`)
        return res.json()
      })
      .then((data) => {
        renderAll(data)
      })
      .catch((err) => {
        console.error("[project-query] 加载失败:", err)
        // 在页面上显示错误提示
        const containers = document.querySelectorAll(QUERY_SELECTOR)
        containers.forEach((c) => {
          c.innerHTML = `<p class="project-error">数据加载失败</p>`
        })
      })
  }

  // 等待 DOM 加载完成
  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", init)
  } else {
    init()
  }
})()
