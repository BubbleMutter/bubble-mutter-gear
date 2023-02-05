#!/bin/bash

<<EOF
how to connect to hidden wifi in the console
<password> has to be included with two double quotation marks

$ nmcli con add type wifi con-name <connect name> ifname wlan0 ssid <ssid>
$ nmcli con modify <connect name> wifi-sec.key-mgmt wpa-psk
$ nmcli con modify <connect name> wifi-sec.psk <password>
$ nmcli con up <connect name>
EOF

SSID="3220_1"
PSK="gdut3220"
IFACE="wlan0"

# for first wifi-card
CON_NAME=${SSID}
nmcli con add type wifi con-name ${CON_NAME} ifname ${IFACE_0} ssid ${SSID}
nmcli con modify ${CON_NAME} wifi-sec.key-mgmt wpa-psk
nmcli con modify ${CON_NAME} wifi-sec.psk ${PSK}
nmcli con up ${CON_NAME}
