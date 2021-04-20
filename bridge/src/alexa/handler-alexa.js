'use strict';

function handleReportState(request) {
    let deviceId = request.header.endpoint;

    // TODO: Check if device is online, then return status
}

module.exports = {
    namespace: 'Alexa',
    handlers: {
        'ReportState': handleReportState
    }
}