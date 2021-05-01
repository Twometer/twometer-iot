'use strict';

const DeviceBus = require('../../core/deviceBus')
const Directives = require("../directives");

module.exports = {
    namespace: 'Alexa.ModeController',
    handlers: {
        SetMode(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            let value = directive.payload.mode;
            DeviceBus.changeProperty(meta.deviceId, meta.property, value);
        }
    }
}