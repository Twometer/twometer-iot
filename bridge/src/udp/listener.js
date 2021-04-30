'use strict';

const Config = require('../config')
const dgram = require('dgram')
const server = dgram.createSocket('udp4')
const logger = require('cutelog.js')
const os = require('os')
const Proto = require('./protocol')

server.on('message', (buffer, remote) => {
    let msg = Proto.parseMessage(buffer);
    if (!msg) return;

    switch (msg.type) {
        case Proto.MsgType.Discovery:
            let signature = msg.params[0];
            if (signature !== 'LumiIoT.Bridge') {
                logger.warn(`Invalid discovery signature ${signature}`)
                return;
            }

            server.send(
                Proto.createMessage(Proto.MsgType.BridgeFound, getIpAddress()),
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
        server.on('error', reject);
        server.bind(Config.UDP_PORT);

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