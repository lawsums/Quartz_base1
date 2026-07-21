---
<%* 
// 1. 所有需要的变量统一在顶部声明（避免重复声明）
let url = await tp.system.clipboard();
let question = await tp.user.getLeetcodeProblem(tp, url, {
  download_imgs: true,
  img_folder: "assets/leetcode_imgs"
});
// let question = await tp.user.getLeetcodeProblem(tp, url, { download_imgs: false });

// console.log(question);
// console.log(question.codeSnippets);

// 1. 筛选出 cpp 对应的字典（find 方法：找到第一个匹配项）
const cppSnippet = question.codeSnippets.find(snippet => snippet.langSlug === 'cpp');

// 2. 安全提取 code（避免找不到时报错）同时自定义all前缀
const cppCode = cppSnippet ? cppSnippet.code : '';
const all = "#include <bits/stdc++.h>\nusing namespace std;\n\n" 

// 3. 打印结果
// console.log(cppCode);

// TODO 
// console.log(question.similarQuestions);

await tp.file.rename(question.questionId + ". " + question.title);

// 提前声明存储标签和提示的变量
let topicTagsText = ""; // 存储分类标签
let hintsText = "";    // 存储提示信息
_%>
Date: <% tp.file.creation_date("YYYY-MM-DD") %>
Link: <% `https://leetcode.com/problems/${question.titleSlug}/` %>
Category: 
<%*
question.topicTags.forEach(item => {
  topicTagsText += `- ${item.name}\n`;
});
tR += topicTagsText; // 输出到模板
_%>
Difficulty: <% question.difficulty %>
SimilarQuestions:  
<%*
let similarQuestionsText = ""; // 存储相似题标题文本
try {
  // 1. 解析 JSON 字符串为数组
  const similarQuestionsArr = typeof question.similarQuestions === 'string' 
    ? JSON.parse(question.similarQuestions) 
    : question.similarQuestions;
  
  // 2. 确认是数组后，遍历拼接格式
  if (Array.isArray(similarQuestionsArr)) {
    similarQuestionsArr.forEach(item => {
      // 拼接 "- 中文标题\n"，每个标题一行
      similarQuestionsText += `- ${item.translatedTitle}\n`;
    });
  }
} catch (error) {
  similarQuestionsText = "- 暂无相似题目\n"; // 解析失败时的默认文本
}

// 3. 输出到模板（tR 是 Templater 的输出变量）
tR += similarQuestionsText;
_%>
Completed: false
---

<%*
let categoryTags = ""; // 用于存储 #标签 集合
question.topicTags.forEach(item => {
  categoryTags += `#${item.name} `; // 每个标签前面加 #
});
tR += categoryTags; // 输出到模板
tR += '\n'
_%>

<%*
// 核心配置与工具函数（统一管理）
const targetFile = tp.config.target_file;
const fs = require('fs');
const path = require('path');

// 1. Neovim 路径配置（必须修改为你的实际路径！）
const nvimExePath = "E:/Program Files/nvim/nvim.exe"; // Windows 示例

// 2. 拼接 .cpp 文件路径（和笔记同名同目录）
const cppFilePath = path.join(
  targetFile.vault.adapter.basePath,
  targetFile.parent.path,
  `${targetFile.basename}.cpp`
);

// 3. 自动创建空白 .cpp 文件（不存在时） 写入codeSinppet
if (!fs.existsSync(cppFilePath)) {
  try {
    fs.writeFileSync(cppFilePath, all + cppCode + "\n", 'utf-8'); 
    console.log(`已创建空白文件：${cppFilePath}`);
  } catch (error) {
    console.error(`创建文件失败：${error.message}`);
  }
}

// 4. 生成 Neovim 打开命令（Windows 适配）
let openNvimCommand;
if (process.platform === "win32") {
  openNvimCommand = `${cppFilePath}`;
} else {
  openNvimCommand = `nvim "${cppFilePath}"`;
}
const nvimLink = `file:///${encodeURIComponent(openNvimCommand)}`;

_%>


```button
name <font color="#548dd4">打开nvim（自动创建.cpp）</font>
type link
action <% nvimLink %>
```

`button-anki-open`   `button-anki-update`

## <% `${targetFile.basename}` %>

<% question.content %>

<%*
// 3. 给另一个已声明的变量赋值（同样不用 let）
question.hints.forEach(item => {
  hintsText += `> [!note]- Hint\n> ${item}\n\n`;
});
// 输出提示内容
hintsText
_%>

## Notes


## Solution 

![[<% `${targetFile.basename}.cpp` %>]]

