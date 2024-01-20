$HOME/.ssh/known_hosts     # remembered servers

$HOME/.ssh/id_rsa          # private key
$HOME/.ssh/id_rsa.pub      # public key

$HOME/.ssh/authorized_keys # authorized public keys for clients

# add a new authorized key to server
cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys

# bypass interaction when first login
sshpass -p "$password" ssh -oStrictHostKeyChecking=no user@$ip

# -C for compressed
sshpass -p "$password" scp -C -oStrictHostKeyChecking=no user@$ip:/tmp/log.txt .
