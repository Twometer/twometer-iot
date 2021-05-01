#!/bin/bash

if [ `whoami` != 'root' ]; then
    echo "Root required"
    exit
fi

# Install services if missing
if [ $(dpkg-query -W -f='${Status}' hostapd 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
	echo "hostapd not installed, installing..."
	apt-get update
	apt-get upgrade
	apt-get install hostapd -y;
fi
if [ $(dpkg-query -W -f='${Status}' dnsmasq 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
	echo "dnsmasq not installed, installing..."
	apt-get install dnsmasq -y;
fi

# Stop services
systemctl stop hostapd
systemctl stop dnsmasq

# Static IP address
echo "
interface wlan0
static ip_address=192.168.0.1/24
denyinterfaces eth0
denyinterfaces wlan0
" > /etc/dhcpcd.conf

# dnsmasq config
mv /etc/dnsmasq.conf /etc/dnsmasq.conf.bak
echo "
interface=wlan0
dhcp-range=192.168.0.100,192.168.0.255,255.255.255.0,24h
" > /etc/dnsmasq.conf

# hostapd config
echo "
interface=wlan0
hw_mode=g
channel=7
wmm_enabled=0
macaddr_acl=0
auth_algs=1
ignore_broadcast_ssid=0
wpa=2
wpa_key_mgmt=WPA-PSK
wpa_pairwise=TKIP
rsn_pairwise=CCMP
ssid=Luminosity
wpa_passphrase=uwu
" > /etc/hostapd/hostapd.conf

# apply config
sed -i 's/#DAEMON_CONF=""/DAEMON_CONF="\/etc\/hostapd\/hostapd.conf"/' /etc/default/hostapd

# work around glitchy raspi wifi
iw wlan0 set power_save off

# enable hostapd

systemctl unmask hostapd.service
systemctl enable hostapd.service

systemctl start hostapd
systemctl start dnsmasq

echo "You should now restart your system."


echo "done"
