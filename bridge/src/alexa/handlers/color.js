'use strict';

const DeviceBus = require('../../core/deviceBus')
const Directives = require("../directives");

module.exports = {
    namespace: 'Alexa.ColorController',
    handlers: {
        SetColor(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            let hsbColor = directive.payload.color;
            DeviceBus.changeProperty(meta.deviceId, meta.property, hsbColor);
        }
    }
}