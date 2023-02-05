[TOC]
1. `openssl enc -list` 对陈加密支持算法一览表

# rsa
```sh
# 生成 1024位 rsa私钥
openssl genrsa -out demo.key 1024
# 从私钥匙中生成 公钥
openssl rsa -in demo.key -pubout -out | tee demo_pub.key
# 使用公钥加密
echo "hello" |  openssl rsautl -encrypt -inkey demo_pub.key -pubin | tee data.en
# 使用私钥解密
cat data.en | openssl rsautl -decrypt -inkey demo.key

# 使用相同公钥 + 相同明文; 多次加密结果是不同的
echo "hello" |  openssl rsautl -encrypt -inkey demo_pub.key -pubin
echo "hello" |  openssl rsautl -encrypt -inkey demo_pub.key -pubin

# 使用相同私钥; 多次生成公钥是唯一的
openssl rsa -in demo.key -pubout | tee demo_pub.key
openssl rsa -in demo.key -pubout | tee demo_pub.key
```
## rsa::padding (4种不同的模式)
```sh
# pkcs#1 v1.5 (不加参数默认模式)
echo "hello" | openssl rsautl -encrypt -inkey demo_pub.key -pubin -pkcs | tee data.en
cat data.en | openssl rsautl -decrypt -inkey demo.key -pkcs

# pkcs#1 oaep
echo "hello" | openssl rsautl -encrypt -inkey demo_pub.key -pubin -oaep | tee data.en
cat data.en | openssl rsautl -decrypt -inkey demo.key -oaep

# ansi x9.31
echo "hello" | openssl rsautl -encrypt -inkey demo_pub.key -pubin -x931 | tee data.en
cat data.en | openssl rsautl -decrypt -inkey demo.key -x931

# no padding
echo "hello" | openssl rsautl -encrypt -inkey demo_pub.key -pubin -raw | tee data.en
cat data.en | openssl rsautl -decrypt -inkey demo.key -raw
```

# hmac (md5 / sha1 / sha512 / sha256)
```sh
# hmac md5
echo "hello" | openssl dgst -md5 -hmac -hex -macopt hexkey:ab12cd34ef56
echo "hello" | openssl md5 -hex -mac HMAC -macopt hexkey:ab12cd34ef56

# hmac sha1
echo "hello" | openssl dgst -sha1 -hmac -hex -macopt hexkey:ab12cd34ef56
echo "hello" | openssl sha1 -hex -mac HMAC -macopt hexkey:ab12cd34ef56

# hmac sha512
echo "hello" | openssl dgst -sha512 -hmac -hex -macopt hexkey:ab12cd34ef56
echo "hello" | openssl sha512 -hex -mac HMAC -macopt hexkey:ab12cd34ef56

# hmac sha256
echo "hello" | openssl dgst -sha256 -hmac -hex -macopt hexkey:ab12cd34ef56
echo "hello" | openssl sha256 -hex -mac HMAC -macopt hexkey:ab12cd34ef56
```

# base64
编码 = `echo hello | openssl base64`
解码 = `echo aGVsbG8K | openssl base64 -d`

1. base64 相同字符串的加密串是唯一的