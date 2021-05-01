'use strict';

const logger = require('cutelog.js')
const UdpServer = require('../udp/listener');
const Proto = require('../udp/protocol')
const Config = require('../config')
const Bus = require('../core/deviceBus')

let onlineDevices = [];

module.exports = {

    _sendPingWave() {
        for (let deviceId in onlineDevices) {
            let device = onlineDevices[deviceId];
            UdpServer.sendMessage(Proto.createMessage(Proto.MsgType.Ping), device.ipAddress);
        }
        onlineDevices = onlineDevices.filter(dev => (Date.now() - dev.lastPong) < 15000);
    },

    initialize() {
        setInterval(this._sendPingWave, Config.PING_WAVE_INTERVAL);
        Bus.emitter.on('login', this.addDevice);
        Bus.emitter.on('heartbeat', this.pongReceived);
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