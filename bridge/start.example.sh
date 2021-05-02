#!/bin/bash

if [ `whoami` != 'root' ]; then
echo "error: requires root"
exit
fi

export LUMI_FIBER_KEY=""
export LUMI_FIBER_URL="wss://fiberserver/lumi/stream"
export LUMI_REST_PORT="80"
export LUMI_MONGO_URL="mongodb://localhost:27017/lumi"
export LUMI_PING_WAVE_INTERVAL="10000"
export LUMI_WIFI_SSID="luminet"
export LUMI_PAIRING_WIFI_SSID="luminet pair"
export LUMI_WIFI_PASS="cutezerotwo"
export LUMI_UDP_PORT=11210
export LUMI_NET_IFACE=eth0

node .
