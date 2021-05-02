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
            if (!device)
                continue;

            UdpServer.sendMessage(Proto.createMessage(Proto.MsgType.Ping), device.ipAddress);
            if (Date.now() - device.lastPong > Config.PING_WAVE_INTERVAL * 2) {
                onlineDevices[deviceId] = undefined;
                logger.info(`Device ${device.deviceId} timed out`);
            }
        }
    },

    initialize() {
        setInterval(this._sendPingWave, Config.PING_WAVE_INTERVAL);
        Bus.emitter.on('login', this.addDevice);
        Bus.emitter.on('heartbeat', this.pongReceived);

        Bus.emitter.on('change', (deviceId, property, value, direction) => {
            let device = onlineDevices[deviceId];
            if (device && direction === Bus.BusDirection.Downstream) {
                let message;
                if (typeof value === 'object') {  // Is a color
                    message = Proto.createMessage(Proto.MsgType.UpdateProperty, property, value.h, value.s, value.b);
                } else { // Can be serialized directly
                    message = Proto.createMessage(Proto.MsgType.UpdateProperty, property, value);
                }
                UdpServer.sendMessage(message, device.ipAddress);
            }
        })
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
        let device = onlineDevices[deviceId];
        if (!device) return;
        device.lastPong = Date.now();
    }

}