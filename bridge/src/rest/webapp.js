'use strict';

const logger = require('cutelog.js')
const express = require('express')
const config = require('../config')
const bodyParser = require('body-parser')
const endpoints = require('./endpoints')

let server;

const app = express();
app.use(bodyParser.json());

function startServer() {
    return new Promise((resolve) => {
        server = app.listen(config.REST_PORT, () => {
            logger.okay(`REST server started on port ${config.REST_PORT}`);
            resolve();
        });
    })
}

async function start() {
    logger.info("Starting web server...");
    await startServer();

    logger.info("Registering endpoints...");
    endpoints.register(app);
}

module.exports = {app, start}