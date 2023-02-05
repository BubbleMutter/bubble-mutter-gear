# container types overview
| name | description |
| :-- | :-- | :-- |
| array | static contiguous array |
| vector | dynamic contiguous array |
| deque | double-ended queue |
| forward_list | singly-linked list |
| list | doubly-linked list |
| set | collection of unique keys, sorted by keys |
| map | collection of key-value pairs, sorted by keys, keys are unique |
| multiset | collection of keys, sorted by keys |
| multimap | collection of key-value pairs, sorted by keys |
| unordered_set | collection of unique keys, unsorted |
| unordered_map | collection of key-value pairs, unsorted |
| unordered_multiset | collection of keys, unsorted |
| unordered_multimap | collection of key-value pairs, unsorted |
| stack | adapts a container to provide stack (LIFO data structure) |
| queue | adapts a container to provide queue (FIFO data structure) |
| priority_queue | adapts a container to provide priority queue |

# container implement overview

# iterator invalidation (迭代器失效)
## 问题背景
1. iterator 本质 (CPU 指令级) 就是容器 data 的 pointer
2. 不同容器的 iteractor 有其自身固定的 递增/递减 方法, 编译时即可确定
3. 由于在 insert / erase / resize 后, 容器 data 有可能发生改变
    1. 这视乎 容器底层的算法实现 导致的
4. 如果在 增删 容器元素前, 缓存了 iterator 那么该 iterator 可能将会失效
    1. 失效的本质是 pointer 指向了非预想的元素 or pointer 变成野指针

## 问题考虑的维度
1. 考虑 增 和 删
2. 考虑 容器类型 (底层实现)
3. 对于 vector, 考虑删除元素的 前面元素 和 后面元素

## 结论概括
1. set map multiset multimap unordered_set unordered_multiset unordered_map unordered_multimap
    1. 除了被删除的迭代器外, 其他迭代器都是不会受到影响
2. list forward_list
    2. 除了被删除的迭代器外, 其他迭代器都是不会受到影响
3. vector
    1. 如果 insert 导致 capacity 扩大, 那么迭代器都将失效
    2. 当 capacity 不变, 位于 insert / erase 之前的迭代器仍有效
    3. 当 capacity 不变, 位于 insert / erase 之后的迭代器将失效
4. deque
    1. 无论 insert / erase, 其他迭代器都失效
5. stack queue priority_queue 是 adaptor, 依赖于用户指定的 容器类型, list 还是 vector

# Container Method
## constructor desctructor operator=
## Iterators
  begin   end 
 rbegin  rend 反向reverse
 cbegin  cend 迭代器const 类型; 
crbegin crend 迭代器const 类型; 反向reverse
## Capacity
  size          = 有效成员个数
  max_size      = 
  empty         = 判断是否为空
  每种对象有自己的方法
## Element access
## Modifers
## Allocatorbook.