'use strict';

const PropertyBus = require('../../core/propertyBus')
const Directives = require("../directives");

module.exports = {
    namespace: 'Alexa.ColorController',
    handlers: {
        SetColor(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            let hsbColor = directive.payload.color;
            PropertyBus.changeProperty(meta.deviceId, meta.property, hsbColor);
        }
    }
}