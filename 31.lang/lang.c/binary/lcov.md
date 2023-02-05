[TOC]
# usage 
```sh
lcov --directory /path/to/source  --gcov-tool gcov --capture --output-file /path/to/output.info
genhtml -o /path/to/html/ /path/to/output.info 2>&1 1> /path/to/genhtml.process.log
```
1. locv --directory 指向的参数需要保证有匹配的 gcno 和 gcda
2. `*.gcno` 在源码编译时; 生成在源码文件相同路径
3. `*.gcda` 在host运行时; 生成在源码gcno相同路径
4. warning: 当除了问题需要排查时; `/proc/$pid/maps` 文件相当重要; 所以 Ctrl-C 前必须保存下来


# 2021/07/21 工作经验
1.  在机子安装valgrind
  + 这个步骤可以统一, 其实就是把valgrind放到一个地方
2.  添加编译选项编译
3.  停掉机子上在跑的进程
4.  把编译出来的镜像放到机子上
5.  运行进程
6.  一大堆风骚操作 发包/打流/伪造IPC/页面操作
7.  停止进程, 用信号停止
8.  docker上拿到机子上的 gcda文件
9.  docker上使用脚本  生成html
10. http服务器 查看当前结果
11. 把当前结果压缩出来

+ 1步骤可以优化成一条命令解决
+ 2 3 4 5 步骤 是不断loop的, 需要一键生成
+ 7 8 9 步骤也应该命令一键完成
+ 还有一种场景是 7 8 9 + 5步骤
+ 10 步骤只是开一个http服务器, py脚本走起
+ 11 这个步骤也应该
