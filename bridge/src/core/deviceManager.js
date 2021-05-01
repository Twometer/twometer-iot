'use strict';

const logger = require('cutelog.js')
const proto = require('../udp/protocol')
const udp = require('../udp/listener');
const config = require('../config')

let onlineDevices = [];

module.exports = {

    _sendPingWave() {
        for (let device in onlineDevices) {
            udp.sendMessage(proto.createMessage(proto.MsgType.Ping), device.ipAddress);
        }
        onlineDevices = onlineDevices.filter(dev => (Date.now() - dev.lastPong) < 15000);
    },

    initialize() {
        setInterval(this._sendPingWave, config.PING_WAVE_INTERVAL)
    },

    addDevice(deviceId, ipAddress) {
        onlineDevices[deviceId] = {
            deviceId: deviceId,
            ipAddress: ipAddress,
            lastPong: Date.now()
        };
        logger.info(`Device ${deviceId} connected at ${ipAddress}.`)
    },

    pongReceived(deviceId) {
        onlineDevices[deviceId].lastPong = Date.now();
    }

}