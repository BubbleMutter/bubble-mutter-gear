#!/bin/sh'
# case 语句 -------------------------------
case $param in
    param_a)
        echo param_a
    ;;
    param_b)
        echo param_b
    ;;
    *)
        echo unknown param
    ;;
esac
# case 语句 -------------------------------

# for 累加
for (( i=1; i<=10; i++ )); do
    echo $i
done # PS: 原始sh不支持, 需要bash
