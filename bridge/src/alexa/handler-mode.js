'use strict';

const db = require('../database')
const Directives = require("./directives");

module.exports = {
    namespace: 'Alexa.ModeController',
    handlers: {
        async SetMode(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            let value = directive.payload.mode;
        }
    }
}