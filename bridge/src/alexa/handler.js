'use strict';

const fiber = require('./fiber')
const { v4: uuid } = require('uuid');

async function initialize() {
    await fiber.connect();
    fiber.setHandler(fiberHandler);
}

async function fiberHandler(message) {
    if (message.payloadVersion !== '3') {
        return;
    }

    let response = {};




    return response;
}

module.exports = {initialize}