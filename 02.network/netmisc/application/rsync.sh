rsync -rdt rsync://IPADDR:RsyncPort/

rsync -azv $src $dst
rsync -azv --delete $src $dst 

# -z 是压缩; 内网传输带宽够就不用 -z
# --delete delete dst extract files