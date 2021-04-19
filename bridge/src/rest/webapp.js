'use strict';

const logger = require('xa')
const express = require('express')
const config = require('../config')
const bodyParser = require('body-parser')

const app = express();
app.set('trust proxy', 1)
app.use(bodyParser.json());

async function start() {
    logger.info("Starting web server...");
    return new Promise((resolve) => {
        app.listen(config.REST_PORT, () => {
            logger.success(`REST server started on port ${config.REST_PORT}`);
            resolve();
        });
    })
}

module.exports = {app, start}