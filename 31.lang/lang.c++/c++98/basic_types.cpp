#include<limits>
using namespace std;

printf("%d\n", numeric_limits<int>::max());
printf("%d\n", numeric_limits<int>::min());
printf("%d\n", numeric_limits<bool>::max());
printf("%d\n", numeric_limits<bool>::min());
printf("%ld\n", numeric_limits<long>::max());
printf("%ld\n", numeric_limits<long>::min());
printf("%hd\n", numeric_limits<short>::max());
printf("%hd\n", numeric_limits<short>::min());
printf("%f\n", numeric_limits<float>::max());
printf("%f\n", numeric_limits<float>::min());
printf("%lf\n", numeric_limits<double>::max());
printf("%lf\n", numeric_limits<double>::min());
printf("%lf\n", numeric_limits<size_t>::max());
printf("%lf\n", numeric_limits<size_t>::min());