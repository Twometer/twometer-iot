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

const DefaultProperties = {
    'Alexa.BrightnessController': 'Light.Brightness',
    'Alexa.ColorController': 'Light.Color',
    'Alexa.PowerController': 'Device.Power'
}

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

function parseDirectiveMeta(directive) {
    return {
        property: directive.header.instance || DefaultProperties[directive.header.namespace],
        deviceId: directive.endpoint.endpointId
    }
}

module.exports = {ErrorType, newMessageId, createErrorResponse, parseDirectiveMeta}