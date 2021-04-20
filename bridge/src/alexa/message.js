'use strict';

const {v4: uuid} = require('uuid');

const ErrorType = {
    EndpointBusy: 'ENDPOINT_BUSY',
    EndpointLowPower: 'ENDPOINT_LOW_POWER',
    EndpointUnreachable: 'ENDPOINT_UNREACHABLE',
    HardwareMalfunction: 'HARDWARE_MALFUNCTION',
    InsufficientPermissions: 'INSUFFICIENT_PERMISSIONS',
    InternalError: 'INTERNAL_ERROR',
    InvalidDirective: 'INVALID_DIRECTIVE',
    InvalidValue: 'INVALID_VALUE',
    NoSuchEndpoint: 'NO_SUCH_ENDPOINT'
};

function createErrorResponse(request, errorType, errorMessage) {
    return {
        event: {
            header: {
                namespace: 'Alexa',
                name: 'ErrorResponse',
                payloadVersion: '3',
                messageId: newMessageId(),
                correlationToken: request.header.correlationToken
            },
            endpoint: {
                endpointId: request.endpoint.endpointId
            },
            payload: {
                type: errorType,
                message: errorMessage
            }
        }
    }
}

function newMessageId() {
    return uuid();
}

module.exports = {ErrorType, newMessageId, createErrorResponse}