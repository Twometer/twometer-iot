'use strict';

const packageInfo = require('../package.json')
const database = require('./db/database')
const webapp = require('./rest/webapp')
const fiber = require('./alexa/fiber')
const config = require('./config')
const logger = require('xa')

logger.success = function (text) {
    logger.custom('OKAY', text, {backgroundColor: 'green'});
}

async function main() {
    logger.info(`Staring ${packageInfo.name} v${packageInfo.version}...`);
    config.load();
    await database.connect();
    await fiber.connect();
    await webapp.start();
}

main().then(() => logger.success('Startup complete'))
    .catch(e => logger.error(`Startup failed: ${e}`));