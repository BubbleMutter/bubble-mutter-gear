#!/bin/bash

# message digest
cat message | openssl dgst -md5
cat message | openssl dgst -sm3
cat message | openssl dgst -sha1
cat message | openssl dgst -sha512

# message encode/decode
openssl base64
openssl base64 -d

rsa() {
    # generate rsa-private
    openssl genrsa -out id_rsa 1024
    # generate rsa-public from rsa-private (always identical for same rsa-private)
    penssl rsa -in id_rsa -pubout -out id_rsa.pub
    # encrypt message with rsa-public
    cat message | openssl pkeyutl -encrypt -inkey id_rsa.pub -pubin -out message.en
    # decrypt message with rsa-private
    openssl pkeyutl -decrypt -inkey id_rsa -in message.en
}