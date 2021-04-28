'use strict';

const db = require('../database')

module.exports = {
    namespace: 'Alexa.SceneController',
    handlers: {
        Activate(directive) {
            let sceneId = directive.endpoint.endpointId;
        },
        Deactivate(directive) {
            let sceneId = directive.endpoint.endpointId;
        }
    }
}