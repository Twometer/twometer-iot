'use strict';

module.exports = {
    namespace: 'Alexa',
    handlers: {
        ReportState(directive) {
            let deviceId = directive.header.endpoint;
            // TODO: Check if device is online, then return status
        }
    }
}