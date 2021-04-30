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
            let signature = msg.params[0];
            if (signature !== 'LumiIoT.Bridge') {
                logger.warn(`Invalid discovery signature ${signature}`)
                return;
            }

            socket.send(
                Proto.createMessage(Proto.MsgType.BridgeHello, getIpAddress()),
                remote.port, remote.address
            )

            return;
        default:
            logger.warn(`Received invalid packet ${msg.type}`)
            return;
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