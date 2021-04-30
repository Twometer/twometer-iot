'use strict';

const logger = require('cutelog.js')
const childProcess = require('child_process')

module.exports = {

    update(ssid, password) {
        if (process.platform === 'win32') {
            logger.warn('WiFi control not supported on Windows - skipping.')
            return;
        }

        return new Promise((resolve, reject) => {
            let command = `/bin/bash ${__dirname}/../../scripts/wifiupdate.sh "${ssid}" "${password}"`
            let child = childProcess.exec(command);

            logger.custom('debug', `Updating WiFi SSID: ${ssid}`)
            child.on('exit', code => {
                if (code === 0)
                    resolve();
                else
                    reject(Error(`WiFi update failed with exit code ${code}`))
            });
        })
    }

}