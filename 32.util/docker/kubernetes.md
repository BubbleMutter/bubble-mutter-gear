https://kubernetes.io/  分布式集群系统

# kubernetes (k8s)
1. 基于容器的集群管理平台
2. 由谷歌 Borg 系统(开发10年了)发展而来
## Cluster (集群)
1. 一个Master节点 (主节点)
2. 一群Node节点 (计算节点)
## Master
1. API Server 是整个系统的对外接口，供客户端和其它组件调用
2. Scheduler  负责对集群内部的资源进行调度
3. Controller manager 负责管理控制器
## Node
1. docker      创建容器
2. kubelet     Pod 的 创建、修改、监控、删除
3. kube-proxy  为Pod对象提供代理
4. fluentd     负责日志收集、存储与查询
5. kube-dns
6. Pod         集群中运行的一个进程 (k8s 最基本的操作单元)
7. service