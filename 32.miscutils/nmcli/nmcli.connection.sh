#!/bin/bash

# obtain through the command line parameters
if [ $# -ne 3 ]; then
	echo "Usage: $0 <interface> <ssid> <psk>"
	exit 1
fi

IFNAME=$1
SSID=$2
PSK=$3

nmcli connection del "$SSID"
nmcli connection add type wifi con-name "$SSID" ifname $IFNAME ssid "$SSID"
nmcli connection modify "$SSID" wifi-sec.key-mgmt wpa-psk
nmcli connection modify "$SSID" wifi-sec.psk "$PSK"
nmcli connection up "$SSID" ifname $IFNAME # it's highly suggested specifying ifname