'use strict';

const packageInfo = require('../package.json')
const database = require('./db/database')
const webapp = require('./rest/webapp')
const config = require('./config')
const logger = require('xa')

async function main() {
    logger.info(`Staring ${packageInfo.name} v${packageInfo.version}...`);
    config.load();
    await database.connect();
    await webapp.start();
}

main().then(() => logger.success('Startup complete'))
    .catch(e => logger.error(`Startup failed: ${e}`));