#include<iostream>
using namespace std;
class barCode {
    private:
        string __barCodeSymbol;
    public:
        barCode(const void *imgData, int width, int height);
        const string barCodeSymbol(void);
};
