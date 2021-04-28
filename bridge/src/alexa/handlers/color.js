'use strict';

const db = require('../../database')
const Directives = require("../directives");

module.exports = {
    namespace: 'Alexa.ColorController',
    handlers: {
        SetColor(directive) {
            let meta = Directives.parseDirectiveMeta(directive);
            let hsbColor = directive.payload.color;
        }
    }
}