#!/bin/bash

# 3 command tools to allow running process in daemon
# 1. nohup
nohup
# 2. setsid
setsid
# 3. disown
disown
# 4. inner
(setsid scripts.sh &)