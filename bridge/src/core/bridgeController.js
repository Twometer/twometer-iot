'use strict';

const logger = require('cutelog.js')
const WiFi = require('./wifi')
const Config = require('../config')
const PropertyBus = require('./propertyBus')

let pairingModeTimeoutId = null;

function propertyChangeHandler(deviceId, property, value, direction) {
    if (direction === PropertyBus.BusDirection.Downstream) {
        logger.info(`[${direction}] Changing property '${property}' on device '${deviceId}' to '${value}'.`)

    }
}

async function initialize() {
    await resetWifi();
    PropertyBus.registerChangeListener(propertyChangeHandler)
}

async function enterPairingMode() {
    pairingModeTimeoutId = setTimeout(leavePairingMode, 60000);
    await WiFi.close();
    await WiFi.setSSID(Config.PAIRING_WIFI_SSID);
    await WiFi.setPassword('');
    await WiFi.open();
}

async function leavePairingMode() {
    if (pairingModeTimeoutId != null) {
        clearTimeout(pairingModeTimeoutId);
        pairingModeTimeoutId = null
    }
    await resetWifi();
}

async function resetWifi() {
    await WiFi.close();
    await WiFi.setSSID(Config.WIFI_SSID);
    await WiFi.setPassword(Config.WIFI_PASS);
    await WiFi.open();
}

module.exports = {initialize, enterPairingMode, leavePairingMode}