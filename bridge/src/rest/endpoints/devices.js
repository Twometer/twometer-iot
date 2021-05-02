'use strict';

const db = require('../../database')
const express = require('express');
const router = express.Router();
const DeviceBus = require('../../core/deviceBus')
const Properties = require('../../core/properties')

router.get('/', async (req, res) => {
    let devices = await db.Device.find();
    res.json(devices.map(dev => {
        return {
            deviceId: dev._id,
            type: dev.type,
            modelName: dev.modelName,
            friendlyName: dev.friendlyName,
            manufacturer: dev.manufacturer,
            description: dev.description
        }
    }));
});

router.get('/:deviceId', async (req, res) => {
    let devices = await db.Device.find({_id: req.params.deviceId});
    if (devices.length === 0) {
        return res.status(404).send();
    }

    let device = devices[0];
    res.json({
        deviceId: device._id,
        type: device.type,
        modelName: device.modelName,
        friendlyName: device.friendlyName,
        manufacturer: device.manufacturer,
        description: device.description,
        properties: device.properties
    });
});

router.put('/:deviceId', async (req, res) => {

});

async function parseDeviceRequest(req) {
    let device = (await db.Device.find({_id: req.params.deviceId}))[0];
    if (!device)
        return null;

    let property = device.properties.filter(p => p.name === req.params.propertyKey)[0];
    if (!property)
        return null;

    return {device, property}
}

router.get('/:deviceId/:propertyKey', async (req, res) => {
    let request = await parseDeviceRequest(req);
    if (!request)
        return res.status(404).send();

    res.json(request.property.currentValue);
});

router.put('/:deviceId/:propertyKey', async (req, res) => {
    let request = await parseDeviceRequest(req);
    if (!request)
        return res.status(404).send();
    if (!Properties.validate(request.property, req.body.value))
        return res.status(400).send();

    DeviceBus.changeProperty(request.device._id, request.property.name, req.body.value);
    res.status(200).send();
});

router.ws('/:deviceId/:propertyKey/stream', async (ws, req) => {
    let request = await parseDeviceRequest(req);
    if (!request)
        return res.status(404).send();

    ws.on('message', (data) => {
        let parsedData = JSON.parse(data);
        if (!Properties.validate(request.property, parsedData))
            return;

        DeviceBus.changeProperty(request.device.deviceId, request.property.name, parsedData.value);
    })
});

module.exports = {
    path: '/devices',
    router
}