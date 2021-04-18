'use strict';

const packageInfo = require('../package.json')
const logger = require('xa')

async function main() {
    logger.info(`Staring ${packageInfo.name} v${packageInfo.version}...`);
}

main().then(() => {
    logger.success('Startup complete');
});