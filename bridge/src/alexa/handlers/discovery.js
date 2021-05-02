'use strict';

const PropertyMapper = require('../propertyMapper')
const Directives = require('../directives')
const Database = require('../../database')

function convertToCapability(property) {
    let map = PropertyMapper.getByProperty(property.name);
    if (!map)
        return undefined;

    return {
        type: "AlexaInterface",
        version: "3",
        interface: map.alexaNamespace,
        properties: {
            supported: [
                {name: map.alexaProperty}
            ],
            proactivelyReported: false,
            retrievable: true
        }
    }
}

function convertToEndpoint(device) {
    return {
        endpointId: device._id,
        manufacturerName: device.manufacturer,
        description: device.description,
        friendlyName: device.friendlyName,
        additionalAttributes: {
            manufacturer: device.manufacturer,
            model: device.modelName
        },
        displayCategories: [device.type],
        cookie: {},
        capabilities: device.properties.map(convertToCapability).filter(p => p !== undefined)
    };
}

module.exports = {
    namespace: 'Alexa.Discovery',
    handlers: {
        async Discover(directive) {
            let devices = await Database.Device.find();
            let response = Directives.createBaseResponse(directive, 'Alexa.Discovery', 'Discover.Response');
            response.event.payload.endpoints = devices.map(convertToEndpoint);
            return response;
        }
    }
}