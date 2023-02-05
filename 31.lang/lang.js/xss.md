本质是 详细用户的输入
用户的输入有 file, input, url
在未检查用户输入, 而直接输出为html, 或 存入db

类型1(反射型): 
    后端拿到 xss字段, 直接渲染到页面
    后端拿到 xss字段, 返回给前端, 前端渲染
类型2(存储型): 
    后端拿到 xss字段, 存放到db中,
    然后每次重新获取db中的值 进行渲染时, 就会被攻击
类型3(DOM型):
    完全发生于 客户端, 由经常由于 eval 导致

典型的是, 留言板/富文本

### xss字段分类:
+ tags: script img link style iframe frame a
+ 属性:  style href src on事件
### xss字段例子:
+ `<script> </script>` 这个太低端了, 浏览器会检测到的
+ `<img src="null" onerror='alert("xss attack")'/>`
+ `<a onmousemove='alert("xss attack")'>`
+ ` ";alert("I am coming again~");" ` 转义成 ` "";alert("xss attack");""; `

### xss防御:
+ cookie有其他验证措施如：Http-Only保证同一cookie不能被滥用
+ https://www.cnblogs.com/unclekeith/p/7750681.html
+ 富文本tag防御 https://github.com/blowsie/Pure-JavaScript-HTML5-Parser
+ 解码输入的数据 https://github.com/mathiasbynens/he, 然后用HTML5-Parser检查
    + 如果没有解码, 用户输入 \u 转义xss字段, 绕过一些防御机制
    + 但是, 浏览器最终必然会对 \u转义的xss字段进行解码

例子1: 留言板盗取访问用户的cookie:
1. 在留言板中 植入 xss代码, 获取 document.cookkie, 上传到自己的server中
2. 等待用户访问 留言板时, 便能获取该用户的 cookie, 其中有token

### 简要总结
+ 一旦在DOM解析过程成出现不在预期内的改变（JS代码执行或样式大量变化时）
  就可能发生XSS攻击
+ XSS分为反射型XSS，存储型XSS和DOM XSS
+ 反射型XSS是在将XSS代码放在URL中，将参数提交到服务器。服务器解析后响应
  在响应结果中存在XSS代码，最终通过浏览器解析执行。
+ 存储型XSS是将XSS代码存储到服务端（数据库、内存、文件系统等）
  在下次请求同一个页面时就不需要带上XSS代码了，而是从服务器读取。
+ DOM XSS的发生主要是在JS中使用eval造成的，所以应当避免使用eval语句。
+ XSS危害有盗取用户cookie，通过JS或CSS改变样式，DDos造成正常用户无法得到服务器响应
+ XSS代码的预防主要通过对数据解码，再过滤掉危险标签、属性和事件等