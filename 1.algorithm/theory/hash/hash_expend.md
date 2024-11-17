# Hash Expand
# Hash Expand: redis'dict implement
+ https://www.cnblogs.com/mfrbuaa/p/5245064.html (注意看后半部分)
+ pseudo code
```c++
struct dict {
    struct dictht;      // 里面包含了mask 用来取模; 代表hash函数
    dictht ht[2];       // ht[0] 旧桶; ht[1] 正在扩展的新桶
    int rehashidx; // immigrate cursor
}

start_rehash() {
    ht[1] = new dictht with (ht[0].size);
    rehashidx = 0;
}

end_rehash() {
    free(ht[0]);
    ht[0] = ht[1];
    ht[1] = NULL;
    d->rehashindex = 0;
}
lookup(key) {
    if (ht[1] == NULL) { // not immigrating
        return __lookup(ht[0], key);
    } else {
        // lookup key from ht[0] and ht[1]
        v = __lookup(ht[0], key);
        if (!v) {
            v = __lookup(ht[1], key);
        }
        rehash ht[0].table[d->rehashindx++] into ht[1]
        return v
    }
}
insert(key) {
    if (isOverload()) {
        start_rehash();
    }
    if (ht[1] == NULL) { // not immigrating
        return __insert(ht[0], key);
    } else {
        __remove(ht[0], key);
        __insert(ht[1], key); // may be update
        rehash ht[0].table[d->rehashindx++] into ht[1]
    }
}
```