# Quartz 项目长期记忆

## 项目概要
- Quartz 静态站点生成器，用于发布 Obsidian 笔记
- 项目路径：`G:\MyPath\quartz`
- Git: `origin → git@github.com:lawsums/Quartz_base1.git`，分支 `v5`

## 同步脚本
- 脚本路径：`scripts/sync-leetcode.sh`
- 源：`E:/Documents/Obsidian/Leetcode`（注意：`E:\Documents\Obsidian_\Cpp\Leetcode` 是符号链接）
- 目标：`G:/MyPath/quartz/content/Leetcode`
- 同步方式：robocopy /MIR 镜像同步 + `npx quartz sync --no-pull` 推送
- 沙箱中 npx 不可用，改用 `node ./quartz/bootstrap-cli.mjs sync --no-pull`
- `.gitignore` 已排除 `scripts/` 和 `*.log`
- robocopy 排除：`*.swp *.swo 1.md 2.md 3.md`（后三者是 Templater 模板，YAML 非法会导致 build 崩溃）
- `npx quartz build --serve` = 本地预览（端口 8080），`npx quartz build` = 仅构建到 public/，`npx quartz sync` = 推送到 GitHub

## 技术备忘
- robocopy 退出码 0-7 = 成功，8+ = 错误，不能用 `set -e`
- `npx quartz sync` = git add + commit + (可选 pull) + push，纯 Git 操作不构建
- `--no-pull` 跳过从 origin/v5 拉取（单用户强制推送工作流，安全）
