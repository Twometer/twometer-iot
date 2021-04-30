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

router.post('/pair', async (req, res) => {
    let descriptor = req.body.device;
    if (!descriptor) {
        return res.status(400).send();
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
    await controller.leavePairingMode();

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