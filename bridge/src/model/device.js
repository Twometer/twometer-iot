'use strict';

function Device(id, modelName, manufacturer, description, type) {
    this.id = id;
    this.modelName = modelName;
    this.manufacturer = manufacturer;
    this.description = description;
    this.type = type;
}

module.exports = Device;