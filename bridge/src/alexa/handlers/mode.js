'use strict';

const PropertyBus = require('../../core/propertyBus')
const Directives = require("../directives");

module.exports = {
    namespace: 'Alexa.ModeController',
    handlers: {
        SetMode(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            let value = directive.payload.mode;
            PropertyBus.changeProperty(meta.deviceId, meta.property, value);
        }
    }
}