'use strict';

const db = require('../database')

async function handleDiscover(request) {
    
}

module.exports = {
    namespace: 'Alexa.Discovery',
    handlers: {
        'Discover': handleDiscover
    }
}