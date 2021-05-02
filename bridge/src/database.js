'use strict';

const mongoose = require('mongoose')
const config = require('./config')
const logger = require('cutelog.js');
const Bus = require('./core/deviceBus')

function connect() {
    return new Promise((resolve) => {
        logger.info("Connecting to database server...");
        mongoose.connect(config.MONGO_URL, {useNewUrlParser: true, useUnifiedTopology: true})
            .then(() => {
                logger.okay("Connected to MongoDB");
                resolve();
            });
    });
}

module.exports = {connection: mongoose.connection, connect}

module.exports.Device = mongoose.model('Device', new mongoose.Schema({
    _id: String,
    type: String,
    modelName: String,
    friendlyName: String,
    manufacturer: String,
    description: String,
    accessToken: String,
    properties: [{name: String, type: String, friendlyName: String, valueRange: String, currentValue: String}]
}, {typeKey: '$type'}));

module.exports.Scene = mongoose.model('Scene', {
    _id: String,
    name: String,
    description: String,
    actions: [{deviceId: String, propertyKey: String, propertyValue: String}]
});

module.exports.getProperties = async function (deviceId) {
    let device = (await module.exports.Device.find({_id: deviceId}))[0];
    if (!device)
        return [];
    return device.properties;
}

Bus.emitter.on('change', async (deviceId, propertyName, value) => {
    let device = (await module.exports.Device.find({_id: deviceId}))[0];
    if (!device)
        return;

    let property = device.properties.filter(prop => prop.name === propertyName)[0];
    if (!property)
        return;

    property.currentValue = JSON.stringify(value);
    await device.save();
})

Bus.emitter.on('login', async (deviceId) => {
    let device = (await module.exports.Device.find({_id: deviceId}))[0];
    if (!device)
        return;

    for (let prop of device.properties) {
        Bus.changeProperty(deviceId, prop.name, JSON.parse(prop.currentValue));
    }
})