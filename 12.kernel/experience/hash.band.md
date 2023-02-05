// hash 值取与操作经常出错; 这里记录一下
# vlan vid VLAN_MASK
```c++
static struct net_device *s_vlan_dev[VLAN_MASK+1];
void set(int vid, struct net_device *dev) {
    s_vlan_dev[vid & VLAN_MASK] = dev;
}
void get(int vid) {
    return s_vlan_dev[vid & VLAN_MASK];
}
```

# hash 取槽
1. 越界示例
```c++
static DEFINE_HASHTABLE(htbl, 10);
hash_val = jhash_1word(key, salt) & HASH_SIZE(htbl);
hlist_head *head = htbl[hash_val];
// 显然这样子 就会越界
```
2. 正确示例
```c++
static DEFINE_HASHTABLE(htbl, 10);
hash_val = (jhash_1word(key, salt) & HASH_SIZE(htbl))-1;
hlist_head *head = htbl[hash_val];
```