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
