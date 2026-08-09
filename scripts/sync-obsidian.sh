#!/bin/bash
#
# sync-obsidian.sh — 将 Obsidian Leetcode 笔记和番剧/漫画卡片镜像同步到 Quartz content 并推送到 GitHub
# 运行环境：Windows + Git Bash
# 用法：  bash scripts/sync-obsidian.sh
#

set -uo pipefail   # 不使用 -e，因为 robocopy 成功退出码（1）会触发 -e 退出

# 禁用 Git Bash (MSYS2) 的路径自动转换，否则 /MIR /XJ 等参数会被转成 G:/Git/MIR 之类的路径
export MSYS_NO_PATHCONV=1
export MSYS2_ARG_CONV_EXCL="*"

# ==================== 配置 ====================
# Quartz 项目根目录
QUARTZ_DIR="G:/MyPath/quartz"
# 脚本所在目录（用于日志）
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LOG_FILE="$SCRIPT_DIR/sync-obsidian.log"

# --- Leetcode 配置 ---
LC_SOURCE="E:/Documents/Obsidian/Leetcode"
LC_TARGET="G:/MyPath/quartz/content/Leetcode"

# --- Projects 配置（番剧/漫画/游戏） ---
PJ_SOURCE="E:/Documents/Obsidian_/Cpp/Projects"
PJ_TARGET="G:/MyPath/quartz/content/projects"

# ==================== 日志设置 ====================
exec > >(tee -a "$LOG_FILE") 2>&1

echo "========================================================"
echo "[$(date '+%Y-%m-%d %H:%M:%S')] Obsidian 同步开始"
echo "  Leetcode:  $LC_SOURCE -> $LC_TARGET"
echo "  Projects:  $PJ_SOURCE -> $PJ_TARGET"
echo "========================================================"

# ==================== 前置检查 ====================
if [ ! -d "$QUARTZ_DIR" ]; then
  echo "错误：Quartz 目录不存在：$QUARTZ_DIR"
  exit 1
fi

# ==================== 步骤 1：同步 Leetcode ====================
echo "[1/6] 镜像复制 Leetcode：$LC_SOURCE -> $LC_TARGET"

if [ ! -d "$LC_SOURCE" ]; then
  echo "警告：Leetcode 源目录不存在：$LC_SOURCE，跳过"
else
  # 排除：*.swp/*.swo（vim交换文件）、1-9.md + LeetCode模板草稿.md（Templater模板，含非法YAML）
  # 注意：robocopy /XF 不支持 [1-9] 字符范围，必须逐个列出
  robocopy "$LC_SOURCE" "$LC_TARGET" /MIR /XJ /XF *.swp *.swo 1.md 2.md 3.md 4.md 5.md 6.md 7.md 8.md 9.md "LeetCode模板草稿.md" /XD .obsidian .git /R:1 /W:1 /NP
  robocopy_exit=$?

  if [ $robocopy_exit -ge 8 ]; then
    echo "错误：robocopy Leetcode 失败，退出码 $robocopy_exit"
    exit 1
  fi
  echo "Leetcode robocopy 完成（退出码 $robocopy_exit）"

  # 清理 ![[*.cpp]] 嵌入（避免代码重复渲染）
  echo "  清理 ![[*.cpp]] 嵌入..."
  find "$LC_TARGET" -name '*.md' -exec sed -i '/!\[\[[^]]*\.cpp\]\]/d' {} +
  echo "  已清理 Leetcode .md 文件"

  # 清理 Templater 模板文件（frontmatter 含 <% 代码，会导致 Quartz 构建崩溃）
  echo "  清理 Templater 模板文件..."
  templater_cleaned=0
  while IFS= read -r -d '' f; do
    if head -3 "$f" | grep -q '<%'; then
      echo "  删除 Templater 文件：$(basename "$f")"
      rm -f "$f"
      templater_cleaned=$((templater_cleaned + 1))
    fi
  done < <(find "$LC_TARGET" -name '*.md' -print0)
  echo "  已清理 $templater_cleaned 个 Templater 文件"
fi

# ==================== 步骤 2：同步 Projects ====================
echo "[2/6] 镜像复制 Projects：$PJ_SOURCE -> $PJ_TARGET"

if [ ! -d "$PJ_SOURCE" ]; then
  echo "警告：Projects 源目录不存在：$PJ_SOURCE，跳过"
else
  robocopy "$PJ_SOURCE" "$PJ_TARGET" /MIR /XJ /XF *.swp *.swo /XD .obsidian .git /R:1 /W:1 /NP
  robocopy_exit=$?

  if [ $robocopy_exit -ge 8 ]; then
    echo "错误：robocopy Projects 失败，退出码 $robocopy_exit"
    exit 1
  fi
  echo "Projects robocopy 完成（退出码 $robocopy_exit）"
fi

# ==================== 步骤 3：生成 Projects JSON ====================
echo "[3/6] 生成 projects-index.json..."

cd "$QUARTZ_DIR" || { echo "错误：无法 cd 到 $QUARTZ_DIR"; exit 1; }

if [ -f "scripts/generate-projects.js" ]; then
  node scripts/generate-projects.js
  node_exit=$?
  if [ $node_exit -ne 0 ]; then
    echo "警告：generate-projects.js 退出码 $node_exit，继续执行"
  fi
else
  echo "警告：scripts/generate-projects.js 不存在，跳过 JSON 生成"
fi

# ==================== 步骤 4：缓存封面图片 ====================
echo "[4/6] 缓存封面图片到本地..."

if [ -f "scripts/cache-covers.js" ]; then
  node scripts/cache-covers.js
  node_exit=$?
  if [ $node_exit -ne 0 ]; then
    echo "警告：cache-covers.js 退出码 $node_exit，继续执行"
  fi
else
  echo "警告：scripts/cache-covers.js 不存在，跳过封面缓存"
fi

# ==================== 步骤 5：生成番剧库页面（内嵌JSON，瞬时渲染） ====================
echo "[5/6] 生成番剧库页面..."

if [ -f "scripts/generate-anime-pages.js" ]; then
  node scripts/generate-anime-pages.js
  node_exit=$?
  if [ $node_exit -ne 0 ]; then
    echo "警告：generate-anime-pages.js 退出码 $node_exit，继续执行"
  fi
else
  echo "警告：scripts/generate-anime-pages.js 不存在，跳过页面生成"
fi

# ==================== 步骤 6：检测变更 ====================
echo "[6/6] 检测 git 变更..."

if [ -z "$(git status --porcelain)" ]; then
  echo "未检测到 git 变更，跳过推送。"
  echo "[$(date '+%Y-%m-%d %H:%M:%S')] 同步结束（无变更）。"
  exit 0
fi

# ==================== 步骤 7：推送到 GitHub ====================
echo "[7/7] 执行：npx quartz sync --no-pull"
npx quartz sync --no-pull
sync_exit=$?

if [ $sync_exit -ne 0 ]; then
  echo "错误：npx quartz sync 失败，退出码 $sync_exit"
  echo "请检查 git status、网络连接和 GitHub SSH 凭证后重试。"
  exit 1
fi

echo "========================================================"
echo "[$(date '+%Y-%m-%d %H:%M:%S')] 同步成功完成！"
echo "========================================================"
