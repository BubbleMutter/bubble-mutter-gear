# DRY (Don’t Repeat Yourself)
拒绝重复代码/设计，重复的地方抽离作为独立函数或库
# Keep It Simple, Stupid (KISS)
保持模块足够简单，大多时候这会带来最好的扩展性

# 
Program to an interface, not an implementation (针对接口编程，而不是实现)
Composition over inheritance(喜欢组合而不是继承)

# Command-Query Separation (CQS)
命令-查询分离原则
查询：当一个方法返回一个值来回应一个问题的时候，它就具有查询的性质
命令：当一个方法要改变对象的状态的时候，它就具有命令的性质

# You Ain’t Gonna Need It (YAGNI)
如果不确认有这样的扩展需求，不要画蛇添足，保持模块性，方案简洁就好了

# Principle of Least Knowledge / Law of Demeter
迪米特法则
最少知识原则

# （面向对象五大原则：S.O.L.I.D 原则）Interface Segregation Principle (ISP) 
http://en.wikipedia.org/wiki/Solid_(object-oriented_design)

# （包设计原则）Common Closure Principle（CCP）– 共同封闭原则

# （包设计原则）Common Reuse Principle (CRP) – 共同重用原则

# Hollywood Principle – 好莱坞原则（不要来找我，我会找你）
好莱坞原则就是一句话——“don’t call us, we’ll call you.”
http://en.wikipedia.org/wiki/Hollywood_Principle
http://en.wikipedia.org/wiki/Inversion_of_Control

# High Cohesion & Low/Loose coupling & – 高内聚， 低耦合
http://en.wikipedia.org/wiki/Coupling_(computer_science)
http://en.wikipedia.org/wiki/Cohesion_(computer_science)
相似的有 Separation of Concerns (SoC) – 关注点分离

# Convention over Configuration（CoC）– 惯例优于配置原则
http://en.wikipedia.org/wiki/Convention_over_Configuration

# Design by Contract (DbC) – 契约式设计
DbC的核心思想是对软件系统中的元素之间相互合作以及“责任”与“义务”的比喻。
http://en.wikipedia.org/wiki/Design_by_contract

# Acyclic Dependencies Principle (ADP) – 无环依赖原则
包之间的依赖结构必须是一个直接的无环图形，也就是说，在依赖结构中不允许出现环（循环依赖）