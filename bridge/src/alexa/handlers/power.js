'use strict';

const PropertyBus = require('../../core/propertyBus')
const Directives = require('../directives')

module.exports = {
    namespace: 'Alexa.PowerController',
    handlers: {
        TurnOn(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            PropertyBus.changeProperty(meta.deviceId, meta.property, true);
        },

        TurnOff(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            PropertyBus.changeProperty(meta.deviceId, meta.property, false);
        }
    }
}