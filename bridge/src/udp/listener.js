'use strict';

const Config = require('../config')
const Proto = require('./protocol')
const logger = require('cutelog.js')
const dgram = require('dgram')
const os = require('os')

const socket = dgram.createSocket('udp4')

socket.on('message', (buffer, remote) => {
    let msg = Proto.parseMessage(buffer);
    if (!msg) return;

    switch (msg.type) {
        case Proto.MsgType.Discovery:
            let target = msg.params[0];
            if (target !== 'LumiIoT.Bridge') {
                break;
            }

            socket.send(
                Proto.createMessage(Proto.MsgType.BridgeHello, getIpAddress()),
                remote.port, remote.address
            )

            break;

        case Proto.MsgType.ReportChange:
            let deviceId = msg.params[0];
            let authToken = msg.params[1];
            let property = msg.params[2];
            let value = msg.params[3];

            break;
        default:
            logger.warn(`Received invalid packet ${msg.type}`)
            break;
    }
});

function start() {
    logger.info('Starting UDP server...')
    return new Promise((resolve, reject) => {
        socket.on('error', reject);
        socket.bind(Config.UDP_PORT);

        logger.okay(`UDP listener started on port ${Config.UDP_PORT}`);
        resolve();
    })
}

function getIpAddress() {
    return os.networkInterfaces()[Config.NET_IFACE]
        .filter(addr => addr.family === 'IPv4')[0]
        .address;
}

module.exports = {start}