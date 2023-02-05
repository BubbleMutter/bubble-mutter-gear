#!/bin/bash
# key-value pair (need bash)
declare -A map
map["key1"]=val1
map+=( ["key2"]=val2 ["key3"]=val3 )