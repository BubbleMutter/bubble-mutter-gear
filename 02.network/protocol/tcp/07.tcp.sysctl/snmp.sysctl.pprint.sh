#!/bin/bash
while read line; do
    snmp+=("$line")
done <&0

i=0
while [ $i -lt ${#snmp[*]} ]; do
    param=( $(echo ${snmp[$i]}   | tr -s ' ' '\n') )
    value=( $(echo ${snmp[$i+1]} | tr -s ' ' '\n') )

    j=1
    while [ $j -lt ${#param[*]} ]; do
        echo ${param[0]} ${param[$j]} ${value[$j]}
        j=$(( $j + 1 ))
    done

    i=$(( $i + 2 ))
done

#############################
# /proc/net/snmp 反人类设计 #
# 这里把列状 table 改为行状  #
#############################