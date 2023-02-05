[TOC]
1. http 超文本传输协议
    1. 协议: 计算机间 数据的{ 通讯/控制/错误处理 } 的约定和规范
    2. 传输: 两点之间双向传输数据约定和规范
    3. 超文本: 文字/图片/音频/视频等"超文本"数据的约定和规范

# 浏览器显示网页所做的工作
1. 发送获取 html 的请求
2. 从服务端取得 html 文档
3. 解析文档, 并发送更多请求
    1. 文件外的 scripts
    2. 文件外的 css
    3. 页面内 href 指向的其他资源 (文件 or 视频)
4. 然后把 文件内 和 文件外 的资源综合一起显示
5. 解析 文件内 和 文件外 的 scripts 的过程中
   可能需要 请求更多资源 以及 更新页面内容

# http 层的代理 (proxies)
1. 透明代理: 不修改 http 内容
2. 不透明代理: 修改 http 内容
3. 代理一般业务:
    1. 缓存 caching (私有缓存: 浏览器特性, 公有缓存: 配置缓存节点加快重新访问)
    2. 过滤 filtering (防火墙)
    3. 负载均衡
    4. 认证 (资源的权限管理 acl)
    5. 日志

# http 特性
1. 简单易读, 方便调试
2. 可扩展, 通过新增 headers 字段即可
3. 无状态, 有会话
    1. 无状态: 多次请求间协议层是没有联系的 (不会有 tcp seq 号之类的)
    2. 有会话: 使用 header::Cookie, 使得多次请求间共享部分信息, 实现形式上的会话

# http 控制参数
1. 缓存 cache: 文档及资源的缓存控制
    1. server 指示 client 哪些文档需要缓存, 缓存多久
    2. client 指示 proxies 忽略哪些文档的缓存, 重新从 server 获取
2. 同源限制  origin constraint:
    1. 浏览器处于安全和隐私的考虑, 只有来自于相同来源的网页才能够获取网站的全部信息
    2. http 可以通过修改 header, 指示开放 "同源限制"
3. 认证 auth : 使得部分资源 只能由 部分用户访问
    1. 使用 header::WWW-Authenticate 进行用户验证
    2. 使用 header::Cookie 实现会话访问, 从而实现资源权限
4. 代理
5. 会话 session: 通过 header::Cookie 记录请求的状态, 从而无状态连接中实现会话

# http 交互
1. 客户端打开 tcp 连续:
    1. 可以一次打开一个
    2. 可以一次打开多个
    3. 可以复用已有连接
2. 客户端发送 http 请求
3. 服务端接收 http 请求
4. 服务端处理 http 请求 (服务端 业务逻辑)
5. 服务端发送 http 响应
6. 客户端接收 http 响应
7. 客户端解析 http 响应 (浏览器 页面解析)
8. 客户端关闭 连接, 或者留作后续请求复用

# http 流程
1. dns 域名解析
2. client 发起, 经过三次握手, 建立 TCP 连接
3. client 向 server 发送 HTTP 请求报文
4. server 处理 HTTP 请求
5. server 返回 HTTP 请求报文
6. client 处理 HTTP 响应, 解析 HTML 内容
   继续向 server 请求 HTML 中的其他资源 (.js, .css 图片)
7. client 渲染页面, 呈现给用户
8. 断开TCP连接

# http 报文
http/2.0 把 header 嵌入到二进制结构 frames 中, 实现 header 的压缩 和 复用,
然而在 server client 解析收发过程中, 仍然存在 http/1.1 的语义项
## http 请求
1. Method   方法: 仅一个, 包括 { GET / POST / HEAD / OPTIONS }
2. Path     路径: 请求资源的路径
3. Version  版本: 仅一个, 包括 { HTTP/1.0, HTTP/1.1, HTTP/2.0 }
4. Headers  头部: 很多个,
5. Body     数据: 部分方法(如 POST) 还需要提供 Body 数据

## http 响应
1. Version  版本: 仅一个, 包括 { HTTP/1.0, HTTP/1.1, HTTP/2.0 }
2. Status Code 状态码: 仅一个, 包括  { 1xx, 2xx, 3xx, 4xx, 5xx }
3. Status Message 状态信息: 仅一个, 一般与 状态码绑定, 可能没有
4. Headers  头部: 很多个
5. Body     数据: 可能是 文本html, 纯文本, JSON, js脚本

## http api (不大懂这些什么玩意, 封顶了交互信息的 接口吗?)
有很多的, 详细查看 https://developer.mozilla.org/en-US/docs/Web/API
1. XMLHttpRequest: 早期最简单的 请求响应 api
2. Fetch API: 新版本的 请求响应 api

# http 标准
https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Resources_and_specifications