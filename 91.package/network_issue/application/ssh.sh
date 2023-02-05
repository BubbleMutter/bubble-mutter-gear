#!/bin/bash

apt install lsh-server
(lshd -p 22344 --root-login &) # no monitor