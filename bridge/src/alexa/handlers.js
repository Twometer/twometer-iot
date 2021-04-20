'use strict';

const fiber = require('./fiber')
const Message = require('./message')

let handlers = [
    require('./handler-alexa'),
    require('./handler-discovery')
];

async function initialize() {
    await fiber.connect();
    fiber.setHandler(fiberHandler);
}

async function fiberHandler(request) {
    if (request.payloadVersion !== '3') {
        return Message.createErrorResponse(request, Message.ErrorType.InvalidDirective, 'Unsupported payload version.');
    }

    for (let handler of handlers) {
        if (handler.namespace === request.header.namespace) {
            let handlerFunc = handler.handlers[request.header.name];
            if (handlerFunc !== null) {
                return await Promise.resolve(handlerFunc(request));
            }
        }
    }

    return Message.createErrorResponse(request, Message.ErrorType.InvalidDirective, `Don't know how to handle ${request.header.namespace}::${request.header.name}.`);
}

module.exports = {initialize}