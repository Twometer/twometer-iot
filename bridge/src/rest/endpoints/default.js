'use strict';

const packageInfo = require('../../../package.json')
const controller = require('../../core/bridgeController')
const Config = require('../../config')
const logger = require('cutelog.js');
const express = require('express');
const {v4: uuid} = require('uuid');
const db = require('../../database')

const router = express.Router();

router.get('/', (req, res) => {
    res.json({
        name: packageInfo.name,
        version: packageInfo.version,
        devices: 0,
        status: controller.isInPairingMode() ? 'pair' : 'online'
    });
});

router.post('/admin', async (req, res) => {
    let action = req.body.action;
    switch (action) {
        case 'enable_pairing':
            await controller.enterPairingMode();
            return res.status(200).send();
        case 'disable_pairing':
            await controller.leavePairingMode();
            return res.status(200).send();
        case undefined:
            return res.status(400).send();
    }
});

function getDefaultValue(property) {
    switch (property.type) {
        case 'EVENT':
            return '';
        case 'BOOLEAN':
            return false;
        case 'NUMBER':
            return 0;
        case 'COLOR':
            return {h: 0, s: 0, b: 0};
        case 'MODE':
            return Object.values(JSON.parse(property.valueRange))[0];
    }
}

router.post('/pair', async (req, res) => {
    let descriptor = req.body;
    if (!descriptor.deviceId) {
        return res.status(400).send();
    }

    for (let property of descriptor.properties) {
        property.currentValue = JSON.stringify(getDefaultValue(property));
    }

    let deviceToken = uuid();
    let device = new db.Device({
        _id: descriptor.deviceId,
        type: descriptor.type,
        modelName: descriptor.modelName,
        friendlyName: descriptor.modelName,
        manufacturer: descriptor.manufacturer,
        description: descriptor.description,
        accessToken: deviceToken,
        properties: descriptor.properties
    });
    await device.save();


    setTimeout(() => {
        controller.leavePairingMode();
    }, 5000);

    logger.info(`New device ${descriptor.deviceId} ('${descriptor.modelName}') connected.`)

    return res.json({
        wifiKey: Config.WIFI_PASS,
        authToken: deviceToken
    })
});

module.exports = {
    path: '/',
    router
}