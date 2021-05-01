'use strict';

const events = require('events');

const BusDirection = {
    Upstream: 'up',
    Downstream: 'down'
}

const emitter = new events.EventEmitter();
const listeners = [];

/**
 * This module provides the message bus for changing properties, reporting values etc.
 * @type {{valueReported(*, *, *): Promise<*>, changeProperty(*, *, *): Promise<void>}}
 */
module.exports = {

    /**
     * Changes the property on the device with the given id
     * @param deviceId  The ID of the device
     * @param property  The name of the property to change
     * @param value     The new value of the property
     * @returns {Promise<void>}
     */
    changeProperty(deviceId, property, value) {
        emitter.emit('change', deviceId, property, value, BusDirection.Upstream);
    },

    /**
     * Reports that a value has changed on a certain device
     * @param deviceId  The ID of the device
     * @param property  The name of the property that has changed
     * @param value     The new value of the property
     * @returns {Promise<void>}
     */
    valueReported(deviceId, property, value) {
        emitter.emit('change', deviceId, property, value, BusDirection.Upstream);
    },

    /**
     * Reports that a new device has logged in
     * @param deviceId  The ID of the device
     * @param ip        The address of the device
     */
    deviceLogin(deviceId, ip) {
        emitter.emit('login', deviceId, ip);
    },

    /**
     * Reports that a device sent its PONG hearbeat
     * @param deviceId The ID of the device
     */
    deviceHeartbeat(deviceId) {
        emitter.emit('heartbeat', deviceId);
    },

    emitter,
    BusDirection

}