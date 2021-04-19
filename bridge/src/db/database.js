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