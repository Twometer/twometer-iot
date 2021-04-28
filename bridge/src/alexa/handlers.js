'use strict';

const FiberStream = require('./fiber-stream')
const Message = require('./directives')
const logger = require('cutelog.js')
const config = require('../config')

let fiber;

let handlers = [
    require('./handler-alexa'),
    require('./handler-discovery'),
    require('./handler-brightness'),
    require('./handler-color'),
    require('./handler-mode'),
    require('./handler-power'),
    require('./handler-scene')
];

async function initialize() {
    fiber = new FiberStream(config.FIBER_URL, config.FIBER_KEY, 'relay')
    await fiber.open();
    fiber.on('message', fiberHandler);

    fiber.on('close', e => {
        logger.error(`Lost connection to the socket [error ${e}]`)
        setTimeout(fiber.open, 10000);
    })

    fiber.on('error', e => {
        logger.error(`Connection to the socket failed [error ${e}]`)
    })
}

async function fiberHandler({directive}) {
    if (directive.header.payloadVersion !== '3') {
        return Message.createErrorResponse(directive, Message.ErrorType.InvalidDirective, 'Unsupported payload version.');
    }

    for (let handler of handlers) {
        if (handler.namespace === directive.header.namespace) {
            let handlerFunc = handler.handlers[directive.header.name];
            if (handlerFunc !== null) {
                return await Promise.resolve(handlerFunc(directive));
            }
        }
    }

    logger.warn(`Received unknown directive ${directive.header.namespace}::${directive.header.name}`)
    return Message.createErrorResponse(directive, Message.ErrorType.InvalidDirective, `Don't know how to handle ${directive.header.namespace}::${directive.header.name}.`);
}

module.exports = {initialize}