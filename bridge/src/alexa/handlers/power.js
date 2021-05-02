'use strict';

const DeviceBus = require('../../core/deviceBus')
const Directives = require('../directives')

module.exports = {
    namespace: 'Alexa.PowerController',
    handlers: {
        TurnOn(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            DeviceBus.changeProperty(meta.deviceId, meta.property, true);
            return Directives.createPropertyResponse(directive, 'ON');
        },

        TurnOff(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            DeviceBus.changeProperty(meta.deviceId, meta.property, false);
            return Directives.createPropertyResponse(directive, 'OFF');
        }
    }
}