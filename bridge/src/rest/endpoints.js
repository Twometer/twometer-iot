'use strict';

const endpoints = [
    require('./default-endpoint'),
    require('./scenes-endpoint')
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