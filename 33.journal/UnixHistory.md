### Unix Pregnant
1. 195x, John McCarthy 首次发表Time-Sharing OS 的 idea
2. CTSS: Compatible Time-Sharing System 项目, MIT领头
3. MULTICS: Multiplexed Information and Comuter Service Project
    + 1965, AT&T 联合MIT 和 GE的一个Project
    + 由于迫切地想要添加更多新功能到CTSS中
    + 各个东家入不敷出
    + MULTICS最终由于AT&T的退出而终结
    + AT&T也是伤痕累累
4. 1969, 原MULTICS成员, 就职于贝尔实验室(AT&T的子公司)的Ken Thompson</br>
带着原来计划中的一些理念(传说是实现file system),</br>
再加上Ken自己file-nodes, tree-file-system, shell stream等想法,</br>
在一台废弃的"小型机"("小"相对当时大型机而言, 但也是价值不菲)PDP-7上</br>
用asm写了一个OS, 或者说是一大堆utilities,</br>
Ken 想在PDP-7上运行游戏"星际穿越"的一大堆utilities.</br>
5. Ken Thompson 还发明了B语言(借鉴BCPL语言),</br>
而早期PDP-7上没有"自身"的compiler和assembler,</br>
但运行在PDP-7上的那些utilities由asm与B语言写成,</br>
且必须用GE里面的"大型机"交叉编译.</br>
PDP-7上的B语言解释器, 也是交叉编译出来的。</br>
6. Denis Ritchie 加入Ken Thompson
7. 1970/1971, 为了给贝尔实验室的专利部门提供"文字处理"服务,</br>
   贝尔实验室新购置小型机PDP-11(比PDP-7强大很多),</br>
    + 原来运行于PDP-7的OS(utilities)用asm移植到PDP-11
    + J.F Ossanna重新实现了roff, 用asm+B语言在PDP-11上运行
    + roff 是一个文档处理程序，可以称得上是首个unix程序
    + 同时，那里运行的roff是真正意义上modern OS的雏形。
    + 不过当时的AT&T 和 贝尔实验室的人应该不能认识到这点吧。
8. UNIX传说是1970年得名</br>
    + 贝尔实验室的人讽刺不能实现最初想法的MULTICS
    + Uniplexed Information and Computer Service
    + UNICS 读作UNIX
    + Note: 值得一提，现代的眼光看来, MULTICS今天也难以实现
9. 1971, Dennis Ritchie与Ken Thompson 开发出C语言，在B语言基础上</br>
    + 传说C是因为BCPL的第二个字母 (如果 C语言不成功, 就会有P语言)
    + 由于C语言的成功, 一直到1973年Ken和Dennis用C语言重写之前的OS
10. 1971年, 运行在PDP-11上的提供roff文档处理服务的OS被称为UNIX-I
    + 值得一提, 里面有fortran compiler 以及一大堆今天看到的基本命令
    + 这些基本命令后来不断扩展，成为posix标准，gnu在coreutils中实现。

### Unix Birth
1. UNIX-II. 1972.06 走出贝尔实验室, 在AT&T的10台大型机上安装
2. UNIX-III. 1973.02 包含了一个C compiler 以及首次实现了"pipe"功能
3. UNIX-IV. 1973.11 首个几乎完全用C语言重写的UNIX 包括kernel到utils
4. UNIX-V. 1974.06 官方数字说UNIX已经成功在超过50个计算机系统上安装
    + 事实上, 同年Ken 和Dennis在C-ACM中发表了UNIX的论文
    + C—ACM全称: Communication of the American Computer
    + 全球各大实验室和研究组织向AT&T请求UNIX的源码
    + 而早在1958年, 为解决"反托拉斯案例"达成和议,</br>
      AT&T被禁止从事计算机销售获利,</br>
      且AT&T必须把"非电话相关技术"提供给其他任何有要求的组织或个人
    + Ken Thompson把磁盘磁带一包包寄出去, 传说里面有纸条"Love, Ken"
5. UNIX-VI. 1975年, 这个版本的UNIX在全世界得到广泛的传播</br>
   AT&T 授权许多大学kernel源码, utils源码，documentation.
6. 值得一提, 1970s早期, 掀起了UNIX黑客文化浪潮,</br>
   从经济学上分析, 这浪潮可能不是出于计算机科学的前沿发展,</br>
   而更着重于推翻之前一个计算级行业几乎所有的"技术假定"和"商业模式",</br>
   如"卡式打孔机", "COBOL", "商务机套装", "IBM批处理大型机"
7. 大型机太贵了，小型机也不便宜，而制作这些小型机尚未有"标准",</br>
   所以在"移植"过程中, 黑客文化浪潮以及美国本土的"反传统文化",</br>
   使得UNIX出现各式各样的变化。

### UNIX -> { BSD and SYSTEM-V }
1. UNIX-VI. 在1975年发布后, Ken Thompson在1975年～1976年间,</br>
以visiting-professor的身份待在University of California at Berkeley(毕业母校).</br>
Ken Thompson在期间为UNIX-VI加了许多tools 和 features:</br>
`B-ffs, c-shell, vi`在新的VAX机子上,</br>
且以BSD协议去发布了一大堆tools, utils, kernel.</br>
2. 与Ken Thompson合作的人有**vi作者**, sun公司创始人, 今天freebsd的man-pages中,</br>
section 2(sys call)结尾看到"...function appeared in version 6 AT&T UNIX"
3. University of California at Berkeley 的 Computer Science Research Group,</br>
一群excellent黑客
4. UNIX-VII. 1979.01发布, 加入了awk, make, sed, tar, Bourne shell, fortran77,</br>
首个modern UNIX.
5. BSD3. 1979.12发布, 首个**完整发布**的BSD, 包活tools, utils, kernel
6. BSD3, UNIX-VII 出现后, UNIX在美国广泛传播和**变异**, 这时还不是PC机.
7. 1983. 美国宪法部针对AT&T的第二次 反垄断 官司胜利, AT&T拆分, UNIX因而可以商业化
8. SYSTEM-III. 1981由AT&T的UNIX Support Group 发布, 不过没有商业化
9. SYSTEM-V. 1983由AT&T的UNIX Support Group发布,</br>
由于AT&T被拆分了, UNIX便可以商业化了,后来以SYSTEM-V命名.
10. SYSTEM-V Release4 (SVR4). 1989发布, 改动太大了, 因而与BSD差异非常大.</br>
以后UNIX就有两大支流BSD和SVR4, Linux是基于SVR4的.

### BSD and TCP/IP
1. 一直以来AT&T和贝尔实验室在网络上的实验性活动投入较少,</br>
Dennis Ritchie对于"通信"功能的焦点在于**单机分时**以及**架构移植**.
2. 1980, DARPA(Defense Advanced Research Project Agency)国防部高级研究计划局</br>
需要对运行着ARPANET的PDP-10进行换代(寿命问题),</br>
3. 同时DEC公司也将用VAX替代PDP-10, DARPA希望在VAX上实现UNIX和新一代网络(TCP/IP),</br>
但DEC的VAX架构运行着专有的VMS系统,</br>
4. DARPA考虑到DEC可能并不愿意改动VMS以支持TCP/IP,</br>
且AT&T的UNIX协议苛刻,</br>
最终选择了BSD作为implementation platform.
5. BSD-4.2. 1983, 首个实现 TCP/IP 的OS distribution.包括API和utils(可能有ifconifg)

### POSIX intro
1. 最初的POSIX可追溯到1984甚至更早,非官方标准组织的厂商们开始,名为usr-group
2. 到了1988年已有的POSIX经过IEEE标准化,IEEE草案IEEE 1003.1-1988</br>
    1988年一开始没有草案没有".1"的,后来的标准才有".1",".2"所以才加上去
3. IEEE 1003.1 又记作POSIX.1 或 POSIX 1003.1
4. 后来ISO/IEC收录为 ISO/IEC 9945-1:1990(内容完全一样)
5. 现在的标准记作:</br>
    1003.1a-年份, 1003.1b-年份, 1003.1c-年份, ...</br>
    1003.2a-年份, 1003.2b-年份, 1003.2c-年份, ...</br>
6. POSIX.1 只是说明接口, 而不是实现, 也不会分syscall和lib-func</br>
    即POSIX.1只有头文件.
7. POSIX.1 标准中定义的头文件完全包含c-std的头文件, 但func存在细节差异</br>
    + 必须的: c-std
    + 必须的: 非c-std
    + 可选: XSI
    + 可选: 非XSI
8. POSIX标准化工作由opengroup的Austin Group维护</br>
    + [Austin Group](http://www.opengroup.org/austin)
9. SUS, Single Unix Specification
    + super set of POSIX, extension of POSIX
10. POSIX中的XSI可选部分, 定义了要得到"XSI证明"所必须实现的interface,</br>
    + 只有得到"XSI证明"才能被称为UNIX, 因为如今只有opengroup持有UNIX商标
    + 即得到"XSI证明"的OS要实现 "POSIX的必须部分" + "XSI可选"