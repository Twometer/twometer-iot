'use strict';

const fiber = require('./fiber')
const Message = require('./message')

async function initialize() {
    await fiber.connect();
    fiber.setHandler(fiberHandler);
}

async function fiberHandler(request) {
    if (request.payloadVersion !== '3') {
        return Message.createErrorResponse(request, Message.ErrorType.InvalidDirective, 'Unsupported payload version.');
    }

    return {};
}

module.exports = {initialize}