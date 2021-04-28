'use strict';

const express = require('express');
const router = express.Router();

router.get('/', (req, res) => {

});

router.get('/:deviceId', (req, res) => {

});

router.put('/:deviceId', (req, res) => {

});

router.get('/:deviceId/:propertyKey', (req, res) => {

});

router.put('/:deviceId/:propertyKey', (req, res) => {

});

router.ws('/:deviceId/:propertyKey/stream', (req, res) => {

});

module.exports = {
    path: '/device',
    router
}