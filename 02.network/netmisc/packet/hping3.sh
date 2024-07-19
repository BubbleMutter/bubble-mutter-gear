#!/bin/bash

# hping3 is to build packet via command line
# send tcp with dip=127.0.0.1 dport=23 syn payload=100bytes
hping3 127.0.0.1 --destport 23 --syn --data 100 --count 1

