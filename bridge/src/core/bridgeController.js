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
    await WiFi.update(Config.PAIRING_WIFI_SSID, '')
}

async function leavePairingMode() {
    if (pairingModeTimeoutId != null) {
        clearTimeout(pairingModeTimeoutId);
        pairingModeTimeoutId = null
    }
    await resetWifi();
}

async function resetWifi() {
    await WiFi.update(Config.WIFI_SSID, Config.WIFI_PASS);
}

module.exports = {initialize, enterPairingMode, leavePairingMode, isInPairingMode: () => pairingModeTimeoutId != null}