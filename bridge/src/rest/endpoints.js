'use strict';

const endpoints = [
    require('./endpoints/default'),
    require('./endpoints/devices'),
    require('./endpoints/scenes')
];

function registerEndpoint(app, endpoint) {
    if (!endpoint.path || !endpoint.router) {
        throw Error("Endpoint must define a path and a router.");
    }

    app.use(endpoint.path, endpoint.router);
}

function register(app) {
    for (let endpoint of endpoints) {
        registerEndpoint(app, endpoint);
    }
}

module.exports = {register}