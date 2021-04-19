'use strict';

const config = require('../config')
const WebSocket = require('ws')
const logger = require('xa')

let socket;

/**
 * This module connects to the Message Queueing API (fibers) which links
 * the local bridge to the Alexa skill.
 *
 * @type {{connect(): Promise<void>}}
 */
module.exports = {

    connect() {
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
            });
            socket.on('message', msg => {
                if (!this.handler) return;
                let response = this.handler(JSON.parse(msg));
                if (!response) return;
                socket.send(JSON.stringify(response));
            });
        })
    },

    setHandler(handler) {
        this.handler = handler;
    }

}