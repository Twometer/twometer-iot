'use strict';

const packageInfo = require('../../package.json')
const express = require('express');
const router = express.Router();

router.get('/', (req, res, next) => {
    res.json({
        name: packageInfo.name,
        version: packageInfo.version,
        status: 'online'
    });
})

module.exports = {
    path: '/',
    router
}