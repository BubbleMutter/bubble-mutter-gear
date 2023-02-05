#!/bin/bash
echo $[ 1 + 1 ]
echo $[ 2 - 1 ]
echo $[ 3 * 1 ]
echo $[ 4 / 1 ]
echo $[ 5 % 2 ]

# 单中括号只能使用比较参 -eq -ne -gt -lt -ge -le 和逻辑参 ( -o -a )
# -o 表示 ||
# -a 表示 &&
if [ 2 -gt 1 ]; then echo yes; fi
if [ 2 -ne 1 ]; then echo yes; fi
# 数值比较 ---------------------------------

# random number
echo $(cat /dev/urandom | tr -dc '0-9' | fold -w10 | head -n1)
