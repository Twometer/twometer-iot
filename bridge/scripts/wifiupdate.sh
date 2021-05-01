#!/bin/bash

if [ `whoami` != 'root' ]; then
    echo "error: requires root"
    exit
fi

NEW_SSID="$1"
NEW_PASS="$2"
WPA="2"

if [ "$NEW_SSID" == "" ]; then
	echo "ssid must be specified"
	exit 
fi

if [ "$NEW_PASS" == "" ]; then
	echo "it's now an open wifi"
	WPA="0"
	NEW_PASS="openopenopenopen" # needs a value to prevent error
fi

echo "
interface=wlan0
hw_mode=g
channel=7
wmm_enabled=0
macaddr_acl=0
auth_algs=1
ignore_broadcast_ssid=0
wpa=${WPA}
wpa_key_mgmt=WPA-PSK
wpa_pairwise=TKIP
rsn_pairwise=CCMP
ssid=${NEW_SSID}
wpa_passphrase=${NEW_PASS}
" > /etc/hostapd/hostapd.conf

systemctl restart hostapd

echo "done"
