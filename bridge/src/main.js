'use strict';

const packageInfo = require('../package.json')

const deviceManager = require('./bridge/device-manager')
const alexaHandler = require('./alexa/handlers')
const database = require('./database')
const webapp = require('./rest/webapp')
const config = require('./config')
const logger = require('xa')

logger.success = function (text) {
    logger.custom('OKAY', text, {backgroundColor: 'green'});
}

async function main() {
    logger.info(`Staring ${packageInfo.name} v${packageInfo.version}...`);
    config.load();
    await deviceManager.initialize();
    await alexaHandler.initialize();
    await database.connect();
    await webapp.start();
}

main().then(() => logger.success('Startup complete'))
    .catch(e => {
        logger.error(e.stack);
        process.exit(1);
    });