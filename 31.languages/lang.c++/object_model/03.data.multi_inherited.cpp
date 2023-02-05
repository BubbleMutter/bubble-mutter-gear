class Point3d {
public:
    // 存在 virtual 接口
protected:
    float _x, _y;
};

class Vertex {
public:
    // 存在 virtual 接口
protected:
    Vertex *next;
};

// 多重继承
// 对象布局为 { 基类1-基类2-基类n-派生类-vptr }
class Vertex3d: public Point3d, public Vertex {
protected:
    float mumble;
};

void usage() {
    Vertex3d v3d;

    /* 多重继承派生的对象;
     * 其地址赋值给 最左边基类指针 时
     * 直接转换即可; 同于单一继承
     */
    Point3d *p3d = &v3d;
    if ((void *)p3d == (void)&v3d) printf("direct give\n");

    /* 多重继承派生的对象;
     * 其地址赋值给 第二个及其后面的基类指针时
     * 需要隐式指针转换(偏移):
     *   pv = (Vertex *)((char *)&v3d + sizeof(Point3d));
     * 实际上是减去 前面类的大小; 获取指针偏移
     */
    Vertex *pv = &v3d;

    /* 多重继承派生类的指针;
     * 其地址赋值给 第二个及其后面的基类指针时
     * 偏移转换前; 要做非空判断
     * pv = pv3d ? ((Vertex *)((char *)&pv3d + sizeof(Point3d))) : nullptr;
     * 因为指针可以为 nullptr, 直接转换会得到 sizeof(Point3d) 的值
     */ 
    Vertex3d *pv3d
    pv = pv3d;

    /* 多重继承派生类的对象;
     * 赋值 第二个及其后面的基类引用时;
     * 不需要 非空判断; 直接偏移即可
     * 因为对象不会是 null
     */
    Point3d &rp3d = v3d;
}
