# Quartz 项目长期记忆

## 项目概要
- Quartz 静态站点生成器，用于发布 Obsidian 笔记
- 项目路径：`G:\MyPath\quartz`
- Git: `origin → git@github.com:lawsums/Quartz_base1.git`，分支 `v5`

## 同步脚本
- 脚本路径：`scripts/sync-obsidian.sh`（原 sync-leetcode.sh 已改名）
- 同步两个源：
  - Leetcode：`E:/Documents/Obsidian/Leetcode` → `content/Leetcode`（排除 1.md~9.md Templater模板，robocopy /XF 不支持 [1-9] 字符范围，必须逐个列出）
  - Projects：`E:/Documents/Obsidian_/Cpp/Projects` → `content/projects`（anime/manga/game 子目录）
- 同步方式：robocopy /MIR 镜像同步 + 生成 JSON + 缓存封面 + `npx quartz sync --no-pull` 推送（6步）
- Leetcode 特有：robocopy 后用 sed 删除 `![[*.cpp]]` 嵌入（避免代码重复渲染）
- Projects 特有：robocopy 后运行 `node scripts/generate-projects.js` 生成 JSON 索引
- 沙箱中 npx 不可用，改用 `node ./quartz/bootstrap-cli.mjs sync --no-pull`
- `.gitignore` 排除 `scripts/rawg-config.json`（API key）和 `*.log`；脚本文件本身纳入版本控制（供 CI 使用）
- `npx quartz build --serve` = 本地预览（端口 8080），`npx quartz build` = 仅构建到 public/，`npx quartz sync` = 推送到 GitHub

## 番剧卡片系统
- 数据来源：Obsidian Bangumi 插件生成的 frontmatter（中文名/日文名/封面/观看状态/评分等）
- `scripts/generate-projects.js`：扫描 .md frontmatter → 输出 `quartz/static/projects-index.json`
- `scripts/cache-covers.js`：下载远程封面到 `quartz/static/covers/`，JSON cover 字段替换为本地路径
- `scripts/generate-anime-pages.js`：读取 JSON → 将番剧数据内嵌到 `content/番剧库.md`（替换 PLACEHOLDER_JSON 占位符）
- `quartz/static/project-query-inline.js`：从页面内嵌 `<script id="projects-data">` 读取 JSON 渲染，零网络请求
- `quartz/static/project-query.js`：fetch 方式渲染，用于游戏库及其他页面（data-project-query）
- `quartz/static/project-styles.css`：响应式网格卡片样式，使用 Quartz 主题变量适配暗色模式
- `content/番剧库.md`：展示页，分在看/想看/已看三个区域，使用 data-project-query-inline
- Head.tsx 已注入 project-styles.css 和 project-query.js
- package.json 的 `prebuild` 会自动运行 generate-projects.js + cache-covers.js + generate-anime-pages.js
- Quartz slugify 规则：空格→`-`、`&`→`-and-`、`%`→`-percent`、`?`/`#`→删除、转小写
- 封面缓存：URL 的 MD5 前16位做文件名，增量下载（已存在则跳过），curl 下载（走系统代理）
- 番剧库渲染方案已优化为内嵌JSON（构建时注入），比 fetch 方案快很多

## 游戏卡片系统
- `scripts/fetch-game.js`：交互式 CLI，使用 RAWG API 搜索游戏 → 生成 Obsidian .md 笔记
- API Key 配置：`scripts/rawg-config.json`（{"apiKey":"xxx"}）或环境变量 `RAWG_API_KEY`
- 注册地址：https://rawg.io/apidocs （免费，2分钟）
- 输出目录：`E:/Documents/Obsidian_/Cpp/Projects/game/`
- 游戏卡片 frontmatter 字段：中文名/外文名/封面/游玩状态/评分/平台/类型/开发商/发售年份/游玩时长/标签
- generate-projects.js 已扩展：游戏字段回退映射（外文名→subtitle、游玩状态→status、评分→rating、开发商→studio、发售年份→year）
- 新增 JSON 输出字段：platform/genre/playtime
- project-query.js 已更新：游戏卡片显示平台/类型/时长标签，"游戏"类别标签
- project-styles.css 已更新：新增游戏状态颜色（已通关/在玩/想玩/搁置）+ game-meta 样式
- `content/游戏库.md`：展示页，分在玩/想玩/已通关/搁置四个区域
- package.json 新增 `"fetch-game": "node scripts/fetch-game.js"` 脚本

## 技术备忘
- robocopy 退出码 0-7 = 成功，8+ = 错误，不能用 `set -e`
- `npx quartz sync` = git add + commit + (可选 pull) + push，纯 Git 操作不构建
- `--no-pull` 跳过从 origin/v5 拉取（单用户强制推送工作流，安全）
- Git Bash (MSYS2) 会把 `/MIR` 等参数转成路径 `G:/Git/MIR`，需 `export MSYS_NO_PATHCONV=1`
- `--serve` 模式：同进程内增量重建（chokidar watch），重启则全量重建，无持久化缓存

## Windows 定时任务
- 任务名：`QuartzSyncObsidian`（Windows 任务计划程序）
- 每周六 12:00 执行，`-StartWhenAvailable`（关机错过则开机补执行）
- 包装器：`scripts/sync-obsidian-auto.bat`（设置 PATH → 调用 `G:\Git\bin\bash.exe` 运行脚本）
- 系统 Git: `G:\Git\bin\bash.exe`，系统 Node: `G:\MyPath\Nodejs\node.exe`
- 管理命令：`schtasks /query /tn QuartzSyncObsidian` 查看状态，`schtasks /run /tn QuartzSyncObsidian` 手动触发

## GitHub Pages 部署
- URL：`https://lawsums.github.io/Quartz_base1/`（项目站点）
- baseUrl：`lawsums.github.io/Quartz_base1`
- 部署方式：GitHub Actions（`.github/workflows/deploy.yml`）
  - 触发：push v5 分支
  - 构建：Node 22 + npm ci + npm run build（prebuild 包含插件安装/项目索引/封面缓存/番剧页面生成）
  - 部署：GitHub 官方 actions/deploy-pages，无需 gh-pages 分支
- GitHub 设置：Settings → Pages → Source 选 "GitHub Actions"
- robots.txt：`quartz/static/robots.txt`（Allow: /，Sitemap 指定）
- **项目站点路径规则**：项目站点 URL 带仓库名前缀（如 `/Quartz_base1/`），所有自定义静态资源引用必须用相对路径 `./static/` 而非绝对路径 `/static/`。绝对路径 `/static/xxx` 在项目站点上解析为 `https://user.github.io/static/xxx`（跳过仓库名）→ 404
