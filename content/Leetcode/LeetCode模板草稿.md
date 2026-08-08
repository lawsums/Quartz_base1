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

await tp.file.rename(question.questionId + ". " + question.title);

// 提前声明存储标签和提示的变量
let topicTagsText = ""; // 存储分类标签
let hintsText = "";    // 存储提示信息
_%>
Date: <% tp.file.creation_date("YYYY-MM-DD") %>
Link: <% `https://leetcode.com/problems/${question.titleSlug}/` %>
Category: 
<%*
// 2. 直接给已声明的变量赋值（不用再写 let）
question.topicTags.forEach(item => {
  topicTagsText += `- ${item.name}\n`;
});
// 输出标签内容
topicTagsText
_%>
Difficulty: <% question.difficulty %>
Completed: false
---


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

// 3. 自动创建空白 .cpp 文件（不存在时）
if (!fs.existsSync(cppFilePath)) {
  try {
    fs.writeFileSync(cppFilePath, "", 'utf-8');
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

// 5. 生成「刷新Solution」的触发脚本（直接嵌入，不依赖 tp.user）
// 关键：把 refreshSolution 函数直接写在脚本里，避免 tp.user 调用
const refreshScript = `
  (async (tp) => {
    const fs = require('fs');
    const path = require('path');
    const targetFile = tp.config.target_file;
    const cppFilePath = path.join(
      targetFile.vault.adapter.basePath,
      targetFile.parent.path,
      \`\${targetFile.basename}.cpp\`
    );

    try {
      // 读取笔记内容和 cpp 文件内容
      const noteContent = await tp.file.content();
      let cppContent = fs.readFileSync(cppFilePath, 'utf-8').trim() || "// 暂无代码实现";

      // 替换 Solution 部分内容
      const updatedContent = noteContent.replace(
        /(## Solution\\n\\n)([\\s\\S]*?)(?=\\n## Notes)/,
        (match, solutionTitle) => {
          return \`\${solutionTitle}\`\`cpp\n\${cppContent}\n\`\`\`;
        }
      );

      await tp.file.modify(updatedContent);
      new Notice("✅ Solution 刷新成功！");
    } catch (error) {
      console.error("刷新失败：", error);
      new Notice(\`❌ 刷新失败：\${error.message}\`);
    }
  })(tp); // 传递 tp 对象到脚本中
`;

// 6. 转义脚本，适配 Advanced URI
const encodedRefreshScript = encodeURIComponent(refreshScript);
const refreshLink = `obsidian://advanced-uri?command=templater%3Aexecute-script&script=${encodedRefreshScript}`;
_%>


<!-- 点击自动创建文件 + 打开 Neovim -->
```button
name <font color="#548dd4">打开nvim（自动创建.cpp）</font>
type link
action <% nvimLink %>
```
```button
name <font color="#6aa84f">刷新Solution代码</font>
type link
action <% refreshLink %>
```

`button-anki-open`   `button-anki-update`

DECK: 面试题-hot100

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

## Solution 

``` include cpp
<% `${targetFile.basename}.cpp` %>
```


## Notes

END

