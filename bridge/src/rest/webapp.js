'use strict';

const logger = require('cutelog.js')
const express = require('express')
const config = require('../config')
const bodyParser = require('body-parser')
const expressWs = require('express-ws')

let server;

const app = express();
app.use(bodyParser.json());
expressWs(app)

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
    require('./endpoints').register(app);
}

module.exports = {app, start}