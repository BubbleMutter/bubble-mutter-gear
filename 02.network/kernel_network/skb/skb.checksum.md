[TOC]
# skb checksum
1. checksum = **sum** + fold + **get complement**
2. packet is valid when `csum_fold(csum_add(pseudo-header-csum, payload-csum) == 0`
3. `skb->csum` stores
   1. or none
   2. or `pseudo-header-csum` only
   3. or `payload-csum` only
4. `skb->ip_summed`  = indicates type of `skb->csum` currently
5. `skb->csum_valid` = stores validated result (rx only)

# rx csum status CHECKSUM_xxx (skb->ip_summed)
CHECKSUM_NONE        = `skb->csum` 不可信
CHECKSUM_COMPLETE    = `skb->csum` 已经计算 `payload-csum` 但未校验
CHECKSUM_PARTIAL     = 已经计算且校验正确 (仅用于 gro 场景)
CHECKSUM_UNNECESSARY = 已经计算且校验正确

# tx csum status CHECKSUM_xxx (skb->ip_summed)
CHECKSUM_NONE        = 协议栈已经计算 csum
CHECKSUM_PARTIAL     = `skb->csum` stores `pseudo-header-csum`, then hardware calculates checksum
CHECKSUM_COMPLETE    =

# core helper function
`inet_compute_pseudo()` = `csum_tcpudp_nofold`
`csum_tcpudp_nofold()` = **sum** pseudo-header
`csum_fold()`    = fold then **get complement**
`csum_unfold()`  = inline function to avoid casting
`tcp_v4_check()` = `csum_tcpudp_magic(...., l4-payload-csum)` = `csum_fold(csum_tcpudp_nofold())`
`tcp_v6_check()` = `csum_ipv6_magic(..., l4-payload-csum)`
`skb_checksum()` = calculate csum

# tcp_v4_rcv() checksum process
1. `tcp_v4_rcv()` 分三步: a) 先简单校验 b)再经过很长的流程 c)最后做完整校验
2. `skb_checksum_init()`:
   1. calculate `pseudo-header-csum`
   2. if CHECKSUM_COMPLETE then do checking, and `skb->csum_valid` stores result
   3. else `skb->csum` stores `pseudo-header-csum`
3. `tcp_checksum_complete()`:
   1. if csum is not unnecessary(`skb_csum_unnecessary`) then do re-checking `__skb_checksum_complete`
4. 主要的代码
    ```c++
    int tcp_v4_rcv() {
    if (skb_checksum_init(skb, IPPROTO_TCP, inet_compute_pseudo)) {
        // csum error
    }
    // other handle ...
        if (tcp_checksum_complete(skb)) {
        // csum error
    }
    }

    __sum16 skb_checksum_init(skb, compute_pseudo) {
    psum = compute_pseudo(skb, ...);
        if (skb->ip_summed == CHECKSUM_COMPLETE) {
            if (!csum_fold(csum_add(psum, skb->csum))) {
                skb->csum_valid = 1;
                return 0;
            }
        }
    skb->csum = psum;
    return 0;
    }

    bool tcp_checksum_complete(struct sk_buff *skb) {
        return !skb_csum_unnecessary(skb) && __skb_checksum_complete(skb);
    }

    int skb_csum_unnecessary(const struct sk_buff *skb) {
        return ((skb->ip_summed == CHECKSUM_UNNECESSARY) || skb->csum_valid || ...);
    }

    // skb->csum must store pseudo-header-csum before calling
    __sum16 __skb_checksum_complete(...) {
        csum = skb_checksum(skb, 0, skb->len, 0);    // calculate whole skb csum
        sum = csum_fold(csum_add(skb->csum, csum));  // do checking
    // ...
    skb->csum = csum;                            // store whole skb csum
    skb->ip_summed = CHECKSUM_COMPLETE;          // indicates type of skb->csum currently
    skb->csum_valid = !sum;                      // store result
        return sum;                                  // return result
    }
    ```

# tcp_v4_rcv() checksum bug
1. tcp 出于性能考虑
   1. 分离 pseudo-header-csum 和 `whole skb csum` 的计算
   2. skb_checksum_init 计算 `pseudo-header-csum`, 然后简单校验
   3. 再经过一段相对较长的流程
   4. tcp_checksum_complete 进行完整的校验
2. 问题在于 `再经过一段相对较长的流程` 中, 其实 skb 已经损坏了,
   如果挂了一些有 bug 的钩子, 在钩子中"平等"对待 损坏的skb,
   重新计算一个正常的 `csum`,
   导致损坏的 skb 足以瞒过 `tcp_checksum_complete`
   最终导致用户态 `recv()` 收包异常

# skb_checksum() usage issue
```c++
// assumpt skb->data pointed to iphdr

// ip 头参与 csum 计算
__wsum zero_csum = skb_checksum(skb, 0, skb->len, 0);

// ip 头不参与 csum 计算
__wsum inet_csum = skb_checksum(skb, l4offset, skb->len-l4offset, 0);
```
1. 如果 ip 头的 csum 是正确, 那么 `csum_fold(zero_csum - inet_csum) == 0`
2. 如果 ip 头的 csum 是正确, 那么下列两者相等
    1. `csum_fold(csum_add(csum_tcpdudp_nofold(pseudo-header, 0), zero_csum))`
    2. `csum_fold(csum_tcpdudp_nofold(pseudo-header, inet_csum))`