#!/bin/bash

# color shell text
# \e["style";"fore_color";"back_color"m
# fore_color { black:30, red:31, green:32, yellow:33,
#               blue:34, magenta:35, cyan:36, white:37 }
# back_color { black:40, red:41, green:42, yellow:43,
#               blue:44, magenta:45, cyan:46, white:47 }
# style      { reset:0, bold:1, light-color:2,
#               reverse:3, baseline:4 }
echo -e '\e[1;32;44mThis is red bold string.\e[0m'
# reset to default color \e[37;40m finally