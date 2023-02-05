#!/bin/bash
ethtool -g $dev
ethtool -G $dev rx $queue_depth # 上调收包队列深度
ethtool -G $dev tx $queue_depth # 上调发包队列深度; 一般调整意义不大

ethtool -l $dev
ethtool -L $dev rx $number
ethtool -L $dev tx $number
ethtool -L $dev combined $number

# overview information
ethtool $dev

# check using driver and firmware
ethtoo -i $dev