#!/bin/bash

# escape monitor
apt install lsh-server
(lshd -p 22344 --root-login &)
$HOME/.ssh/known_hosts     # remembered servers

$HOME/.ssh/id_rsa          # private key for local
$HOME/.ssh/id_rsa.pub      # public key  for local

$HOME/.ssh/authorized_keys # authorized public keys for remote clients
# add a new authorized key
cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys

# bypass interaction when first login
sshpass -p "$password" ssh -oStrictHostKeyChecking=no user@$ip

# -C for compressed
sshpass -p "$password" scp -C -oStrictHostKeyChecking=no user@$ip:/tmp/log.txt .

# ssh proxy server
ssh -fCNR "3000:127.0.0.1:22" server_user@server_ip     # local host
ssh -fCNL "*:4000:127.0.0.1:3000" server_user@127.0.0.1 # proxy server
ssh -p 4000 local_host_user@server_ip            # access from outside

# 研究了好久的命令
# 实现 1. 本地脚本远端执行
#     2. 远端输出重定向到远端文件
#     3. 远端输出回显到本地
export ssh_options="-o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -o LogLevel=ERROR"
sshpass -p $pass ssh $ssh_options $user@$addr "bash -x - 2>&1 | tee -a $log" < $task_script