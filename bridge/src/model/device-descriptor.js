'use strict';

function DeviceDescriptor(id, modelName, manufacturer, description, type) {
    this.id = id;
    this.modelName = modelName;
    this.friendlyName = modelName;
    this.manufacturer = manufacturer;
    this.description = description;
    this.type = type;
}

module.exports = DeviceDescriptor;