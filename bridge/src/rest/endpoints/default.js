'use strict';

const packageInfo = require('../../../package.json')
const controller = require('../../core/bridgeController')
const Config = require('../../config')
const express = require('express');
const {v4: uuid} = require('uuid');
const router = express.Router();

router.get('/', (req, res) => {
    res.json({
        name: packageInfo.name,
        version: packageInfo.version,
        devices: 0,
        status: 'online'
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
    let deviceId = req.body.deviceId;
    let deviceToken = uuid();
    // TODO Add device to database

    return res.json({
        wifiKey: Config.WIFI_PASS,
        authToken: deviceToken
    })
});

module.exports = {
    path: '/',
    router
}