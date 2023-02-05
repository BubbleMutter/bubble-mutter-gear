[TOC]
# netfilter
+ netfilter    实现 hookpoint 的 nf_hook_ops 调用机制
  + iptables   实现 table (raw/mangle/nat/filter) 的 nf_hook_ops
  + conntraack 实现 nf_hook_ops
+ each netns init
  + iptables = dup each table, append nf_hook_ops to netfilter hookpoint
  + conntrack = append nf_hook_ops to netfiler hookpoint
+ netfilter runtime
```c++
struct net {
	struct netns_nf		nf;
}
struct netns_nf	{
	struct nf_hook_entries __rcu *hooks_ipv4[NF_INET_NUMHOOKS];
	struct nf_hook_entries __rcu *hooks_ipv6[NF_INET_NUMHOOKS];
	struct nf_hook_entries __rcu *hooks_arp[NF_ARP_NUMHOOKS];
	struct nf_hook_entries __rcu *hooks_bridge[NF_INET_NUMHOOKS];
}
struct nf_hook_entries {
	u16				num_hook_entries;
	struct nf_hook_entry		hooks[];
}

// 协议栈具体协议执行节点
NF_HOOK(NFPROTO_BRIDGE, NF_BR_POST_ROUTING, ...)
NF_HOOK(NFPROTO_BRIDGE, NF_BR_LOCAL_IN, ...)
NF_HOOK(NFPROTO_IPV4, NF_INET_PRE_ROUTING, ..)
NF_HOOK(NFPROTO_IPV6, NF_INET_PRE_ROUTING, ..)
// ...

// iterate hookpoint in protostack, every hookpoint has an dynamic array of hookfn
NF_HOOK(FORWARD/LOCAL_IN/LOCAL_OUT/PREROUTING/POSTROUTING) {
    nf_hook() {
         // get one of nf_hook_entries in netns_nf
        hook_head[FORWARD/LOCAL_IN/LOCAL_OUT/PREROUTING/POSTROUTING];
        nf_hook_slow(hook_head) {
            for (; s < e->num_hook_entries; s++) {
		        verdict = nf_hook_entry_hookfn(&e->hooks[s], skb, state) {
                    entry->hook(entry->priv, skb, state);
                }
            }
        }
    }
}
```

# netfilter init
1. `nf_register_net_hooks`
```c++
nf_register_net_hooks(struct net *net, const struct nf_hook_ops *reg, unsigned int n) {
	for (i = 0; i < n; i++) {
		err = nf_register_net_hook(net, &reg[i]) {
            // return one of nf_hook_entries[hook_point] in netns_nf
            pp = nf_hook_entry_head(net, pf, reg->hooknum, reg->dev);
            // ...
            p = nf_entry_dereference(*pp);
            // append nf_hook_ops into the dynamic array of nf_hook_entries
            new_hooks = nf_hook_entries_grow(p, reg);
        }
    }
}
```

## iptables init (raw/mangler/filter/nat)
```c++
struct net {
	struct netns_xt		xt;
};
struct netns_xt {
	struct list_head tables[NFPROTO_NUMPROTO];
};
// 单个 table 在特定 hookpoint 上才有 nf_hook_ops
// xt_hook_ops_alloc 根据 table 的配置, 生成对应 nf_hook_ops[]
rawtable_ops = xt_hook_ops_alloc(&packet_raw, iptable_raw_hook);
mangle_ops = xt_hook_ops_alloc(&packet_mangler, iptable_mangle_hook);
filter_ops = xt_hook_ops_alloc(&packet_filter, iptable_filter_hook);
iptable_nat_table_init() {
    // nat 表的 nf_hook_ops 不走 ipt_register_table 初始化
    ipt_nat_register_lookups() {
        for (i = 0; i < ARRAY_SIZE(nf_nat_ipv4_ops); i++) {
            nf_nat_ipv4_register_fn(net, &nf_nat_ipv4_ops[i]) {
                nf_nat_register_fn() {
                    nf_register_net_hooks(); }
            }
        }
    }
}

// iptables register
ipt_register_table(net, table, repl, rawtable_ops, ...);         // iptables_raw.c
ipt_register_table(net, &packet_mangler, repl, mangle_ops, ...); // iptables_mangle.c
ipt_register_table(net, &packet_filter, repl, filter_ops, ...);  // iptables_filter.c
ipt_register_table(net, &nf_nat_ipv4_table, repl, NULL, ...);    // iptables_nat.c

/**
 * @doc: 每次 netns 创建并初始化时, 每个 table (raw/mangle/nat/filter) 初始化
 *       不同 netns 的相同 table 有独立的 xt_table
 *       不同 netns 的相同 table 共用 nf_hook_ops
 *       every nf_hook_ops indicates the hookpoint of netfilter
 * @param table table 的 default 配置
 * @param repl  table 每次 netns 创建并初始化时, 复制一份 repl
 *              经过 translate_table(repl) 生成 newinfo
 *              经过 xt_register_table(table, newinfo) 生成 new_table
 *              最终 new_table 传出到 res
 *              new_table 作为实际 netns 的 xt_table 上下文
 * @param ops   table 的 netfilter 回调
 * @param res   传输参数, 即 net->ipv4.iptable_raw/iptable_mangle/iptable_filter/nat_table
 */
int ipt_register_table(struct net *net, const struct xt_table *table,
		       const struct ipt_replace *repl,
		       const struct nf_hook_ops *ops, struct xt_table **res) {
	struct xt_table_info *newinfo;
	struct xt_table *new_table;
    // ...
	newinfo = xt_alloc_table_info(repl->size);
    // ...
	ret = translate_table(net, newinfo, loc_cpu_entry, repl); // repl to newinfo
	new_table = xt_register_table(net, table, &bootstrap, newinfo) {
        // xt.tables[table-af] 一般有 raw/mangle/nat/filter 4个成员
        list_add(&table->list, &net->xt.tables[table->af]);
    }
	WRITE_ONCE(*res, new_table);
	ret = nf_register_net_hooks(net, ops, hweight32(table->valid_hooks))
}
```

## nf_nat init
```c++
struct nf_hook_ops nf_nat_ipv4_ops[] = { /* ... */ };
struct nf_hook_ops nf_nat_ipv6_ops[] = { /* ... */ };
nf_nat_ipv4_register_fn() {
    nf_nat_register_fn() {
        nf_register_net_hooks();
    }
}
```

## conntrack init nf_hook_ops
```c++
struct nf_hook_ops ipv4_conntrack_ops[] = { /* ... */ };
struct nf_hook_ops ipv6_conntrack_ops[] = { /* ... */ };
nf_ct_netns_get() {
  nf_ct_netns_inet_get() {
    nf_ct_netns_do_get() {
        // ...
        nf_register_net_hooks(net, ipv4_conntrack_ops, ...);
        // ...
        nf_register_net_hooks(net, ipv6_conntrack_ops, ...);
        // ...
    }
  }
}
```


# iptables match and action
```c++
struct xt_af {
	struct list_head match;
	struct list_head target;
    // ...
};
// corresponded to the family of netfilter
static struct xt_af *xt[arp/ip/ip6];
// append action
xt_register_targets(struct xt_target *target, unsigned int n) {
	for (i = 0; i < n; i++) {
		err = xt_register_target(&target[i]) {
            u_int8_t af = target->family;
            list_add(&target->list, &xt[af].target);
        }
    }
}
// append match
xt_register_matches(struct xt_match *match, unsigned int n) {
	for (i = 0; i < n; i++) {
		err = xt_register_match(&match[i]) {
            u_int8_t af = match->family;
            list_add(&match->list, &xt[af].match);
        }
    }
}
```

# iptables match action runtime (ipt_do_table)
```c++
iptable_filter_hook() { return ipt_do_table(skb, state, state->net->ipv4.iptable_filter); }
iptable_mangle_hook() { return ipt_do_table(skb, state, state->net->ipv4.iptable_mangle); }
iptable_raw_hook() { return ipt_do_table(skb, state, state->net->ipv4.iptable_raw); }
iptable_nat_do_chain() { return ipt_do_table(skb, state, state->net->ipv4.nat_table); }

ipt_do_table(struct sk_buff *skb, const struct nf_hook_state *state, struct xt_table *table) {
    // ...
    struct ipt_entry *e; // 对应一条 iptables rule
    // ...
    e = get_entry(table_base, private->hook_entry[hook /* hookpoint */]);
	do {
        const struct xt_entry_target *t;
		const struct xt_entry_match *ematch;
        // ip_packet_match() 用以匹配 rule 的非 --match 的条件
        // 包括: --in-interface, --out-interface, --ipv4, --ipv6, --proto, --source, --destination
		if (!ip_packet_match(ip, indev, outdev, &e->ip, acpar.fragoff)) {
 no_match:
			e = ipt_next_entry(e);
			continue;
		}
        // xt_ematch_foreach 这个匹配 rule 的 --match 条件
		xt_ematch_foreach(ematch, e) {
			acpar.match     = ematch->u.kernel.match;
			acpar.matchinfo = ematch->data;
			if (!acpar.match->match(skb, &acpar))
				goto no_match;
		}
        // 从 ipt_entry 取得 target
        t = ipt_get_target_c(e);

        // NULL, 表示默认 action: return/continue/drop/goto
        if (!t->u.kernel.target->target) {
            // 这个 block 里面, 最终一定是 break or continue
        }
        // 非NULL, 对应其他 action; 如 mark/nat/log/... 此类
		verdict = t->u.kernel.target->target(skb, &acpar);
        // ...
    } while (!acpar.hotdrop)
}
```

# iptables match (example: xt_tcpudp.c)
```c++
static struct xt_match tcpudp_mt_reg[] __read_mostly = {
	{
		.name		= "tcp",
		.family		= NFPROTO_IPV4,
		.checkentry	= tcp_mt_check,
		.match		= tcp_mt,
		.matchsize	= sizeof(struct xt_tcp),
		.proto		= IPPROTO_TCP,
		.me		= THIS_MODULE,
	}, // ... udp, ipv6,
};
tcpudp_mt_init(void) {
	return xt_register_matches(tcpudp_mt_reg, ARRAY_SIZE(tcpudp_mt_reg));
}
```

## iptables action (example: xt_nat.c)
```c++
xt_snat_target_v0(struct sk_buff *skb, const struct xt_action_param *par) {
	enum ip_conntrack_info ctinfo;
	struct nf_conn *ct;
    // ...
	ct = nf_ct_get(skb, &ctinfo);
    // ...
	xt_nat_convert_range(&range, &mr->range[0]);
    // ...
    // nf_nat_setup_info
	return nf_nat_setup_info(ct, &range, NF_NAT_MANIP_SRC);
}

struct xt_target xt_nat_target_reg[] __read_mostly = {
	{
		.name		= "SNAT",  // for userspace command line -j SNAT
		.target		= xt_snat_target_v0,
		.family		= NFPROTO_IPV4,
		.table		= "nat",   // filter by iptable_nat
		.hooks		= (1 << NF_INET_POST_ROUTING) | (1 << NF_INET_LOCAL_IN),
	},
    // ....
};
```
1. conntrack module setup `struct nf_conn` inside skb
2. netfilter traverse hookfn util nf_hook_ops of `iptable_nat.c`
    1. `iptable_nat.c:iptable_nat_do_chain()` traverse match and action table util xt_NAT.c
    2. `xt_NAT.c:xt_snat_target_v*()` setup `struct nf_conn` inside skb
3. netfilter traverse hookfn util nf_hook_ops of `nf_nat_proto.c`
    1. `nf_nat_proto.c:nf_nat_ipv4_fn(),nf_nat_ipv6_fn()` call `nf_nat_inet_fn()`
    2. `nf_nat_inet_fn()` > `nf_nat_packet()` > `nf_nat_manip_pkt()`
    3. `nf_nat_manip_pkt()` modifies data in skb according to the `struct nf_conn`