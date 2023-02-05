#!/usr/bin/python3
# -*- coding: utf-8 -*-

# 重整化客户端 默认返回的 文件名

import sys, os
import string

# check parameter
if (len(sys.argv) < 2):
    print("usage: {} filename".format(sys.argv[0]))
    sys.exit(-1)

path_full = os.path.basename(sys.argv[1]) # problem id
path_part = path_full.split('.')          # problem title
path_word = path_part[1].split('-')       # problem language

for index, word in enumerate(path_word):
    if all([c == 'i' for c in word]): # version word
        path_word[index] = path_word[index].upper()
    else:
        path_word[index] = path_word[index].capitalize()

# rebuild filename
nice_name = "{:05}. {}.{}".format(int(path_part[0]), ' '.join(path_word), path_part[2])

# do rename on filesystem
os.rename(sys.argv[1], os.path.join(os.path.dirname(sys.argv[1]), nice_name))
