'use strict';

const Database = require('../../database')
const DeviceBus = require('../../core/deviceBus')
const Directives = require("../directives");

module.exports = {
    namespace: 'Alexa.BrightnessController',
    handlers: {
        SetBrightness(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            let brightness = directive.payload.brightness;
            DeviceBus.changeProperty(meta.deviceId, meta.property, brightness);
            return Directives.createPropertyResponse(directive, brightness);
        },

        async AdjustBrightness(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            let delta = directive.payload.brightnessDelta;
            let newValue = await Database.getPropertyValue(meta.deviceId, meta.property) + delta
            DeviceBus.changeProperty(meta.deviceId, meta.property, newValue);
            return Directives.createPropertyResponse(directive, newValue);
        }
    }
}