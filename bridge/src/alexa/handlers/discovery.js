'use strict';

const PropertyMapper = require('../propertyMapper')
const Directives = require('../directives')
const Database = require('../../database')

function convertToCapability(property) {
    let map = PropertyMapper.getByProperty(property.name);
    if (!map && property.type !== 'MODE')
        return undefined;

    if (!map) {
        map = {
            alexaNamespace: 'Alexa.ModeController',
            alexaProperty: 'mode'
        }
    }

    let capability = {
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

    if (property.type === 'MODE') {
        let valueRange = JSON.parse(property.valueRange);

        capability.properties.nonControllable = false;
        capability.instance = property.name;
        capability.capabilityResources = {
            friendlyNames: [{
                '@type': 'text',
                value: {
                    text: property.friendlyName,
                    locale: 'de-DE'
                }
            }],
        };
        capability.configuration = {
            ordered: false,
            supportedModes: []
        }

        for (let key in valueRange) {
            if (!valueRange.hasOwnProperty(key))
                continue;
            if (key === '_id')
                continue;

            let friendlyName = valueRange[key];

            capability.configuration.supportedModes.push({
                value: key,
                modeResources: {
                    friendlyNames: [{
                        '@type': 'text',
                        value: {
                            text: friendlyName,
                            locale: 'de-DE'
                        }
                    }]
                }
            })
        }
    }

    return capability;
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