#!/bin/bash

# system version
cat /etc/issue

# chkconfig
sudo cp ${bootup_script} /etc/init.d
sudo chkconfig ${bootup_script} on