'use strict';

const PropertyMapper = require('./propertyMapper')
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

function createPropertyResponse(directive, value) {
    return {
        event: {
            header: {
                namespace: 'Alexa',
                name: 'Response',
                payloadVersion: '3',
                messageId: newMessageId(),
                correlationToken: directive.header.correlationToken
            },
            endpoint: {
                endpointId: directive.endpoint.endpointId
            },
            payload: {}
        },
        context: {
            properties: [
                {
                    namespace: directive.header.namespace,
                    name: PropertyMapper.getByNamespace(directive.header.namespace).alexaProperty,
                    value: value,
                    timeOfSample: new Date().toISOString(),
                    uncertaintyInMilliseconds: 500
                }
            ]
        }
    }
}

function createBaseResponse(directive, namespace, name) {
    return {
        event: {
            header: {
                namespace,
                name,
                payloadVersion: '3',
                messageId: newMessageId(),
            },
            payload: {}
        }
    };
}

function newMessageId() {
    return uuid();
}

function parseDirectiveMeta(directive) {
    return {
        property: directive.header.instance || PropertyMapper.getByNamespace(directive.header.namespace).lumiProperty,
        deviceId: directive.endpoint.endpointId
    }
}

module.exports = {ErrorType, createPropertyResponse, createErrorResponse, createBaseResponse, parseDirectiveMeta}