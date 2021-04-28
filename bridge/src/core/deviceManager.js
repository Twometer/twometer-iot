'use strict';

const config = require('../config')

let connectedDevices = [];

module.exports = {

    async _sendPingWave() {
        let onlineDevices = {};

        for (let deviceId in devices) {
            if (await this.isReachable(deviceId)) {
                onlineDevices[deviceId] = devices[deviceId];
            }
        }

        devices = onlineDevices;
    },

    async initialize() {
        setInterval(this._sendPingWave, config.PING_WAVE_INTERVAL)

        for (let protoId in protocols) {
            let proto = protocols[protoId];
            await proto.initialize();
            proto.setCallback(this._onDeviceAdded);
        }
    },

    addDevice(deviceDescriptor, ipAddress) {
        devices[deviceDescriptor.id] = {
            descriptor: deviceDescriptor,
            ipAddress
        };
    },

    async isReachable(deviceId) {
        let device = devices[deviceId];
        return await protocols[device.protocolVersion].ping();
    }

}