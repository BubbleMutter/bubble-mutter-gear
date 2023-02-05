[TOC]
# ip 分片组包超时销毁逻辑
```s
ip_local_deliver
> ip_defrag
> ip_find
> inet_frag_find
> inet_frag_create
> inet_frag_alloc
    > timer_setup(ip_expire)
```
1. 从 netfilter::

## 1. 4元组 连接创建时, 创建定时器
## 2. 每次有 4元组 的分片时, 更新定时器
## 3. 完成组片后, 销毁定时器
## 4. 超时没能完成组片, 销毁该碎片包 (不是销毁连接)
