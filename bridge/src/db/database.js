'use strict';

const mongoose = require('mongoose')
const config = require('../config')
const logger = require('xa');

function connect() {
    return new Promise((resolve) => {
        logger.info("Connecting to database server...");
        mongoose.connect(config.MONGO_URL, {useNewUrlParser: true, useUnifiedTopology: true})
            .then(() => {
                logger.success("Connected to MongoDB");
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
    capabilities: [{key: String, type: String, direction: String, friendlyName: String, valueRange: String}],
    currentState: [{key: String, value: String}]
});

module.exports.Scene = mongoose.model('Scene', {
    _id: String,
    name: String,
    description: String,
    actions: [{deviceId: String, capKey: String, capValue: String}]
})