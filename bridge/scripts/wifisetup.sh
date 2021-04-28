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
country_code=DE
hw_mode=g
channel=0
wmm_enabled=0
macaddr_acl=0
auth_algs=1
ignore_broadcast_ssid=0
wpa=2
wpa_key_mgmt=WPA-PSK
wpa_pairwise=TKIP
rsn_pairwise=CCMP
ssid=__ssid__
wpa_passphrase=__password__
" > /etc/hostapd/hostapd.conf

# apply config
sed -i 's/#DAEMON_CONF=/DAEMON_CONF="\/etc\/hostapd\/hostapd.conf" #/' /etc/default/hostapd

echo "You should now restart the system."

echo "done"