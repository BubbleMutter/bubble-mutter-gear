# async scaning
flag=$(mktemp)
i=0
(while [ -f $flag ]; do
    i=$[ $i + 1 ]
    # scaning command
    date -R > output/date.$(printf %010d $i)
done &)
# do task command
rm $flag