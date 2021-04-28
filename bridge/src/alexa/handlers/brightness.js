'use strict';

const PropertyBus = require('../../core/propertyBus')
const Directives = require("../directives");

module.exports = {
    namespace: 'Alexa.BrightnessController',
    handlers: {
        SetBrightness(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            let brightness = directive.payload.brightness;
            PropertyBus.changeProperty(meta.deviceId, meta.property, brightness);
        },

        AdjustBrightness(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            let delta = directive.payload.brightnessDelta;
        }
    }
}