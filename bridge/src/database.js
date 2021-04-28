'use strict';

const mongoose = require('mongoose')
const config = require('./config')
const logger = require('cutelog.js');

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

module.exports.Device = mongoose.model('Device', {
    _id: String,
    type: String,
    modelName: String,
    friendlyName: String,
    manufacturer: String,
    description: String,
    properties: [{key: String, type: String, friendlyName: String, valueRange: String, currentValue: String}]
});

module.exports.Scene = mongoose.model('Scene', {
    _id: String,
    name: String,
    description: String,
    actions: [{deviceId: String, propertyKey: String, propertyValue: String}]
});