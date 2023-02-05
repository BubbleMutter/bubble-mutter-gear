#!/bin/bash
#
# 过滤有问题的内存泄露报告
#
# 遍历文件夹中每个文件
# 过滤出 Invalid, definitelty 等字段的 BOI (block of interest)
# 处理每个 BOI
#   1. 是否有Invalid
#   2. 是否definitely为0 bytes
#   3. 无Invalid 且 definitely为0 bytes的, 不做后续处理
# TODO:
# 处理每个 BOI 中的 sub-block
#      1. 是否是 Invalid
#      2. sub-block中的字段是否符合已知问题

sed_cmd=""
sed_cmd=$sed_cmd'/^=\{1,\}.*[0-9]\{1,\}=\{1,\} .* definitely /,/^=\{1,\}.*[0-9]\{1,\}=\{1,\} *$/p; '
sed_cmd=$sed_cmd'/^=\{1,\}.*[0-9]\{1,\}=\{1,\} * Invalid /,/^=\{1,\}.*[0-9]\{1,\}=\{1,\} *$/p; '

# 遍历每个文件
find . -name "*.log" | while read file; do
    roi_has_Invalid=0
    roi_has_zero_bytes=0
    roi_sub_blk_idx=0

    # 获取BOI
    BOI=$(sed -n -e "$sed_cmd" $file)

    # 处理每个BOI
    # 1. 是否有definitely为0 bytes
    echo -e "$BOI" | egrep -q "^=*[0-9]*=* *definitely lost: 0 bytes"
    if [ $? == 0 ]; then roi_has_zero_bytes=1; fi

    # 2. 是否有Invalid
    echo -e "$BOI" | egrep -q "^=*[0-9]*=*.* Invalid "
    if [ $? == 0 ]; then roi_has_Invalid=1; fi

    msg=""
    msg=$msg" ""$file"
    msg=$msg" ""zero:$roi_has_zero_bytes"
    msg=$msg" ""Invalid:$roi_has_Invalid"
    echo -e "$msg"

    # 3. 无Invalid 且 definitely为0 bytes的, 不做后续处理
    if [ $roi_has_zero_bytes == 1 ] && [ $roi_has_Invalid == 0 ]; then continue; fi

    # 处理每个roi_sub_blk
    echo -e "$BOI" | while read line; do
        # roi_sub_blk 的开始
        if [ $roi_sub_blk_idx == 0 ]; then
            roi_sub_blk_idx=1
            roi_sub_blk=""
        fi

        roi_sub_blk=$roi_sub_blk"$line""\n"

        # roi_sub_blk 的结束
        echo -e "$line" | egrep -q "^=*[0-9]*=* *$"
        if [ $? == 0 ]; then
            echo -e "$roi_sub_blk"
            roi_sub_blk_idx=0
            # perform task on roi_sub_blk
            # TODO:
        fi
    done

    # 处理每个BOI --end
done
