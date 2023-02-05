# client
```sh
# remember server [optional]
$HOME/.ssh/known_hosts

# private key
$HOME/.ssh/id_rsa
$HOME/.ssh/id_rsa_web      # for another server
$HOME/.ssh/id_rsa_database # for another server
```

# server
```sh
# add a new authorized key to server
cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys

# authorized public keys
~/.ssh/authorized_keys

# public key
~/.ssh/id_rsa.pub
```