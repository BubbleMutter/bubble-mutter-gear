# 问题定义
1. 至少有三条直角边的平面封闭图形称为多边形 (polygon)
2. 一个多边形如果它任意两个点的连线都不包含该多边形以外的点, 称为凸多边形 (convex polygon)
3. 一个平面点集的凸包(convex hull) 是指包含S 的最小凸多边形
   该多边形的顶点 (即角) 称为S的极点 (extreme point)
4. 给定一个平面点集S `vector<pair<int64_t, int64_t>>` 计算凸包(convex hull)

# 算法思路
## 步骤1: 退化
1. 点集S 点个数少于3个, 返回S
2. 点集S 3个点在一条直线上, 返回两个端点

## 步骤2: 极角排序
1. 选取点集S 内部的一个点X (由点集S中 `(Y-max + Y-min)/2` 以及 `((X-max + X-min)/2)` 求得)
2. 点X向下作一条垂直线
3. 依次计算点集S中每个点与 垂直线的逆时针夹角, 即极角 (polar)
4. 先根据家夹角大小递增排序, 再根据与点X的距离递增排序

## 步骤3: 删除非极点的点
1. 根据极角顺序, 依次遍历点集S的3个连续点, 判断逆时针夹角是否大于 180 度
2. 大于: 遍历下一个点
3. 不大于: 删除第二个点
```c++
for (x = sorted_list.front(), rx = x->next; rx != sorted_list.front(); ) {
    rrx = rx->next;
    if (x, rx, rxx 逆时针夹角大于 180 度) {
        x = rx;
        rx = rrx;
    } else {
        sorted_list.erase(rx);
        // 此处形同回退操作; 从而确定 x 节点是否为有效极点
        rx = x;
        x = x->prev;
    }
}
```
