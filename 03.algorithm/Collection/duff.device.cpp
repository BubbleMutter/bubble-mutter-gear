void duff_device(void) {
    for(unsigned i = 0; i < 10000; ++i) {
        doSomething();
    }
    // can be optimized to
    for(unsigned i = 0; i < 1000; ++i) {
        doSomething();
        // ... doSomething(); // 8 times
        doSomething();
    }
}


/**
 * @note 对于大数据量的for, 
 *       当 for 操作 CPU 消耗较大
 *       进行一定程度的展开能提高效率
 *       一般地 使用8为展开因子
 */