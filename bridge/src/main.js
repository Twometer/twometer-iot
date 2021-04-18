'use strict';

const packageInfo = require('../package.json')
const config = require('./config')
const logger = require('xa')

async function main() {
    logger.info(`Staring ${packageInfo.name} v${packageInfo.version}...`);
    config.load();
}

main().then(() => logger.success('Startup complete'))
    .catch(e => logger.error(`Startup failed: ${e}`));