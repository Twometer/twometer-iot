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
            return {h: 0, s: 0, b: 0};
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
                return !isNaN(value.h) && !isNaN(value.s) && !isNaN(value.b);
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
        message = Proto.createMessage(Proto.MsgType.UpdateProperty, property, value.h, value.s, value.b);
    } else { // Can be serialized directly
        message = Proto.createMessage(Proto.MsgType.UpdateProperty, property, value);
    }
    return message;
}

module.exports = {getDefaultValue, validate, serialize}
