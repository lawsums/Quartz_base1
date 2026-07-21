const path = require('path');
const fs = require('fs');
let tp = null;

const headersList = {
  Accept: "*/*",
  "User-Agent": "leet-get-problem",
  "Content-Type": "application/json",
};

/**
 * 获取 LeetCode 中文版（leetcode.cn）题目
 *
 * @async
 * @param {Object} tp_object - Templater 对象
 * @param {string} url - LeetCode 中文版题目 URL（如 https://leetcode.cn/problems/two-sum/）
 * @param {boolean} [download_imgs=false] - 是否下载图片
 * @param {string} [img_folder="assets/leetcode_images"] - 图片保存文件夹
 * @returns {Promise<Object>} - 包含中文信息的题目对象
 */
async function getLeetcodeProblem(tp_object, url, download_imgs = true, img_folder = "leetcode/images") {
  tp = tp_object;

  const question = await fetchLeetcodeCNAPI(url); // 改为调用中文版接口
  // 优先使用中文内容：translatedContent（中文题目描述）、translatedTitle（中文标题）
  question.content = tp.obsidian.htmlToMarkdown(question.translatedContent || question.content);
  question.title = question.translatedTitle || question.title; // 标题用中文
  question.topicTags = question.topicTags.map(tag => ({
    name: tag.translatedName || tag.name // 标签用中文
  }));

  if (download_imgs) {
    question.content = await getImages(question.content, img_folder, question.questionId);
  }
  return question;
}

/**
 * 调用 LeetCode 中文版 GraphQL 接口
 */
async function fetchLeetcodeCNAPI(url) {
  const titleSlug = extractCNProblemSlug(url); // 适配中文版 URL 解析
  if (!titleSlug) {
    console.error("无效的 LeetCode 中文版 URL");
    return null;
  }

  const gqlBody = {
    query: `query questionData($titleSlug: String!) {
            question(titleSlug: $titleSlug) {
            questionId
            questionFrontendId
            boundTopicId
            title
            translatedTitle # 中文标题
            titleSlug
            content
            translatedContent # 中文题目描述
            isPaidOnly
            difficulty
            likes
            dislikes
            isLiked
            similarQuestions
            contributors {
                username
                profileUrl
                avatarUrl
                __typename
            }
            langToValidPlayground
            topicTags {
                name
                translatedName # 中文标签
                slug
                __typename
            }
            companyTagStats
            codeSnippets {
                lang
                langSlug
                code
                __typename
            }
            stats
            hints # 提示（中文版已自带中文）
            solution {
                id
                canSeeDetail
                __typename
            }
            status
            sampleTestCase
            metaData
            judgerAvailable
            judgeType
            mysqlSchemas
            enableRunCode
            enableTestMode
            envInfo
            libraryUrl
            __typename
            }
        }`,
    variables: { titleSlug: titleSlug },
  };
  const bodyContent = JSON.stringify(gqlBody);

  // 关键修改：中文版 GraphQL 接口地址（leetcode.cn 而非 leetcode.com）
  const response = await tp.obsidian.request({
    url: "https://leetcode.cn/graphql",
    method: "POST",
    body: bodyContent,
    headers: headersList,
  });

  const problem = await JSON.parse(response);
  if (!problem.data.question) {
    throw new Error("未找到题目，请检查 URL 是否正确（需是 leetcode.cn 的题目链接）");
  }
  return problem.data.question;
}

/**
 * 适配中文版 URL：提取 titleSlug（支持两种格式）
 * 支持 URL：
 * 1. https://leetcode.cn/problems/two-sum/
 * 2. https://leetcode.cn/problems/two-sum/description/
 */
function extractCNProblemSlug(url) {
  // 中文版 URL 正则：匹配 /problems/ 后的字符串（直到下一个 / 为止）
  const regex = /leetcode\.cn\/problems\/([^/]+)/;
  const match = url.match(regex);
  if (match && match[1]) {
    return match[1];
  } else {
    throw new Error("无效的 LeetCode 中文版 URL！请使用类似 https://leetcode.cn/problems/two-sum/ 的链接");
  }
}

/**
 * 下载题目中的图片，替换为本地相对路径（逻辑不变，适配中文版图片 URL）
 */
async function getImages(content, img_folder, prefix) {
  const img_folder_path = path.join(vaultPath(), img_folder);
  mkdir(img_folder_path);
  const note_folder_path = path.join(vaultPath(), tp.config.target_file.parent.path);

  for (const match of findImgLinks(content)) {
    const img_url = match.groups.url;
    // 处理中文版图片 URL 可能的前缀（如 https://assets.leetcode.cn/...）
    const img_filename = `${prefix}_${path.basename(img_url).replace(/\?.*$/, "")}`; // 去除 URL 参数
    const img_path = path.join(img_folder_path, img_filename);
    await downloadImage(img_url, img_path);
    const relative_img_path = path.relative(note_folder_path, img_path);
    content = content.replace(img_url, relative_img_path.replace(/\\/g, "/"));
  }

  return content;
}

// 以下函数逻辑不变，直接复用
async function downloadImage(url, filepath) {
  const response = await tp.obsidian.requestUrl({
    url: url,
    method: 'GET',
    responseType: 'arraybuffer',
    headers: { ...headersList },
  });

  if (response.status === 200) {
    fs.writeFileSync(filepath, Buffer.from(response.arrayBuffer), 'binary');
  } else {
    console.error(`下载图片失败：${url}`);
  }
}

function vaultPath() {
  return tp.config.target_file.vault.adapter.basePath;
}

function mkdir(dir) {
  if (!fs.existsSync(dir)) {
    fs.mkdirSync(dir, { recursive: true });
  }
}

function findImgLinks(content) {
  const regex = /!\[.*?\]\((?<url>.*?)\)/g;
  return content.matchAll(regex);
}

// 导出函数（保持和模板调用一致）
module.exports = getLeetcodeProblem;