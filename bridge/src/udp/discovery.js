'use strict';

const Config = require('../config')
const dgram = require('dgram')
const server = dgram.createSocket('udp4')
const logger = require('cutelog.js')
const os = require('os')

server.on('message', (msg, rinfo) => {
    console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
});

function start() {
    logger.info('Starting discovery server...')
    return new Promise((resolve, reject) => {
        server.on('error', reject);
        server.bind(Config.UDP_PORT);

        logger.okay(`Discovery listener started on port ${Config.UDP_PORT}`);
        resolve();
    })
}

function getIpAddress() {
    return os.networkInterfaces()[Config.NET_IFACE]
        .filter(addr => addr.family === 'IPv4')[0]
        .address;
}

module.exports = {start}