'use strict';

const logger = require('cutelog.js')
const WiFi = require('./wifi')
const PropertyBus = require('./propertyBus')

function propertyChangeHandler(deviceId, property, value, direction) {
    logger.info(`[${direction}] Changing property '${property}' on device '${deviceId}' to '${value}'.`)
}

function initialize() {
    WiFi.open();
    PropertyBus.registerChangeListener(propertyChangeHandler)
}

function enterPairingMode() {

}

function leavePairingMode() {

}

module.exports = {initialize, enterPairingMode, leavePairingMode}