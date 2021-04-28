'use strict';

const Directives = require('../directives')
const db = require('../../database')

module.exports = {
    namespace: 'Alexa.PowerController',
    handlers: {
        TurnOn(directive) {
            let meta = Directives.parseDirectiveMeta(directive);

        },

        TurnOff(directive) {
            let meta = Directives.parseDirectiveMeta(directive);

        }
    }
}