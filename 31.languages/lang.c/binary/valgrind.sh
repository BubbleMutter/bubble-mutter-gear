export GCOV_PREFIX_STRVGCV_ADDR=6
export VALGRIND_LIB=valgrind-$version/lib/valgrind
valgrind --extra-debuginfo-path=/path/to/symbol --leak-check=full --undef-value-errors=no --keep-debuginfo=yes --log-file="${procname}.%p.val.txt" ./$procname 2>&1 1 >> ${proname}.stdout

ubuntu 下要替换两个库; 这一拷贝就不能回头了
cp ./valgrind_arm64/lib/ld-2.22.so /lib/ld-2.22.so
cp ./valgrind_arm64/lib/libc-2.22.so /lib/libc-2.22.so

