'use strict';

const config = require('../config')

const protocols = {
    2: require('../net/protocol-v2'),
    3: require('../net/protocol-v3')
}

let devices = {}

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

    _onDeviceAdded(deviceDescriptor, protocolVersion, endpoint) {
        devices[deviceDescriptor.id] = {
            descriptor: deviceDescriptor,
            protocolVersion,
            endpoint
        };
    },

    async initialize() {
        setInterval(this._sendPingWave, config.PING_WAVE_INTERVAL)

        for (let protoId in protocols) {
            let proto = protocols[protoId];
            await proto.initialize();
            proto.setCallback(this._onDeviceAdded);
        }
    },

    async isReachable(deviceId) {
        let device = devices[deviceId];
        return await protocols[device.protocolVersion].ping();
    }

}