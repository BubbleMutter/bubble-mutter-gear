# show keys
gpg --list-keys
# gen a key (remember the username)
gpg --gen-key
# export public key
gpg --export -a $username 
