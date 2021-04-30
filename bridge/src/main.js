'use strict';

const packageInfo = require('../package.json')
const config = require('./config')
const logger = require('cutelog.js')
const chalk = require('chalk')

const bridgeController = require('./core/bridgeController')
const alexaClient = require('./alexa/alexaClient')
const udpListener = require('./udp/listener')
const database = require('./database')
const webapp = require('./rest/webapp')

async function main() {
    logger.configure('debug', chalk.gray)

    logger.info(`Starting ${packageInfo.name} v${packageInfo.version}...`);
    config.load();

    // Initialize the main controller
    await bridgeController.initialize();

    // Connect to various services
    await alexaClient.connect();
    await database.connect();

    // Start the servers
    await udpListener.start();
    await webapp.start();
}

main().then(() => logger.okay('Startup complete'))
    .catch(e => {
        logger.error(e.stack);
        process.exit(1);
    });