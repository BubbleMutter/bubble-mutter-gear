#!/bin/bash
header=$1
[ ! -f $header ] && continue
macro_like=$(echo $(basename $header) | tr [:lower:]. [:upper:]_)
macro=__${macro_like}__
sed -i "s|\r$||" $header
sed -i "/$macro_like/d" $header

sed -i "1i#ifndef $macro" $header
sed -i "1a#define $macro" $header
sed -i "\$a#endif \/\/ $macro" $header
