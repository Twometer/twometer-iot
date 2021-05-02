'use strict';

const PropertyMapper = require('../propertyMapper')
const Directives = require('../directives')
const Database = require('../../database')

function convertPropertyValue(map, value) {
    let parsedValue = JSON.parse(value);
    if (map.alexaProperty === 'powerState')
        return parsedValue ? 'ON' : 'OFF';
    else
        return parsedValue;
}

module.exports = {
    namespace: 'Alexa',
    handlers: {
        async ReportState(directive) {
            let deviceId = directive.endpoint.endpointId;
            let device = (await Database.Device.find({_id: deviceId}))[0];
            if (!device)
                return Directives.createErrorResponse(directive, Directives.ErrorType.EndpointUnreachable, 'Unknown device');

            let response = Directives.createBaseResponse(directive, 'Alexa', 'StateReport');
            response.context = {properties: []}

            for (let prop of device.properties) {
                let map = PropertyMapper.getByProperty(prop.name);
                if (!map)
                    continue;

                response.context.properties.push({
                    namespace: map.alexaNamespace,
                    name: map.alexaProperty,
                    value: convertPropertyValue(map, prop.currentValue),
                    timeOfSample: new Date().toISOString(),
                    uncertaintyInMilliseconds: 500
                })
            }

            return response;
        }
    }
}