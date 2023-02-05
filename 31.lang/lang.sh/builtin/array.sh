# 数组 ----------------------------
array=(violin cello piano)
# 1. 获取数组成员个数
echo array size is ${#array[*]}
# 2. 获取数组指定成员
echo array[1] item is [ ${array[1]} ]
# 3. 获取数组指定成员的 字符串长度
echo array[1] size is ${#array[1]}
# 4. 遍历数组
for item in ${array[@]}; do
    echo [loop array with "for"]: $item
done
# 4. 扩展数组
array+=("head" "body" "tail")
# 数组 end -----------------------