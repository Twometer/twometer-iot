'use strict';

const packageInfo = require('../../../package.json')
const express = require('express');
const router = express.Router();

router.get('/', (req, res) => {
    res.json({
        name: packageInfo.name,
        version: packageInfo.version,
        devices: 0,
        status: 'online'
    });
});

router.post('/admin', (req, res) => {

});

router.post('/pair', (req, res) => {

});

module.exports = {
    path: '/',
    router
}