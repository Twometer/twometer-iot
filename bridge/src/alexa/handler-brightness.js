'use strict';

const db = require('../database')
const Directives = require("./directives");

module.exports = {
    namespace: 'Alexa.BrightnessController',
    handlers: {
        SetBrightness(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            let brightness = directive.payload.brightness;
        },

        AdjustBrightness(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            let delta = directive.payload.brightnessDelta;
        }
    }
}