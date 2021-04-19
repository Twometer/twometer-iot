'use strict';

const config = require('../config')
const WebSocket = require('ws')
const logger = require('xa')

let socket;

function connect() {
    return new Promise((resolve, reject) => {
        logger.info(`Connecting to fiber at '${config.FIBER_URL}'...`);

        socket = new WebSocket(config.FIBER_URL, {
            headers: {
                Authorization: `X-FiberAuth ${config.FIBER_KEY}`
            }
        });

        socket.on('open', () => {
            logger.success("Connected to the fiber.")
            resolve();
        });
        socket.on('error', e => {
            reject(e);
        })
    })
}

async function send(json) {
    socket.send(JSON.stringify(json));
}

module.exports = {connect, send}