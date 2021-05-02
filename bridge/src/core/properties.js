'use strict';

const Proto = require('../udp/protocol')

function getDefaultValue(property) {
    switch (property.type) {
        case 'EVENT':
            return '';
        case 'BOOLEAN':
            return false;
        case 'NUMBER':
            return 0;
        case 'COLOR':
            return {hue: 0, saturation: 0, brightness: 0};
        case 'MODE':
            return Object.keys(JSON.parse(property.valueRange))[0];
    }
}

function validate(property, value) {
    switch (property.type) {
        case 'EVENT':
            return true;
        case 'BOOLEAN':
            return value === true || value === false;
        case 'NUMBER':
            return !isNaN(value);
        case 'COLOR':
            try {
                return !isNaN(value.hue) && !isNaN(value.saturation) && !isNaN(value.brightness);
            } catch (e) {
                return false;
            }
        case 'MODE':
            return Object.keys(JSON.parse(property.valueRange)).includes(value);
    }
}

function serialize(property, value) {
    let message;
    if (typeof value === 'object') {  // Is a color
        message = Proto.createMessage(Proto.MsgType.UpdateProperty, property, value.hue, value.saturation * 255, value.brightness * 255);
    } else { // Can be serialized directly
        message = Proto.createMessage(Proto.MsgType.UpdateProperty, property, value);
    }
    return message;
}

module.exports = {getDefaultValue, validate, serialize}
