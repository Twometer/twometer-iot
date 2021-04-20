'use strict';

const db = require('../database')

const BusDirection = {
    Upstream: 'up',
    Downstream: 'down'
}

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
    async changeProperty(deviceId, property, value) {
        let device = await db.Device.findOne({_id: deviceId}).exec();
        if (device != null) {
            listeners.forEach(listener => listener(deviceId, property, value, BusDirection.Downstream));

            // TODO Update property on the device

        }
    },

    /**
     * Reports that a value has changed on a certain device
     * @param deviceId  The ID of the device
     * @param property  The name of the property that has changed
     * @param value     The new value of the property
     * @returns {Promise<void>}
     */
    async valueReported(deviceId, property, value) {
        listeners.forEach(listener => listener(deviceId, property, value, BusDirection.Upstream));
    },

    /**
     * Registers a value change listener on the bus
     * @param listener A function with the signature (deviceId, property, value, direction)
     */
    registerChangeListener(listener) {
        if (listener == null || typeof listener !== 'function')
            return;

        listeners.push(listener);
    },

    BusDirection

}