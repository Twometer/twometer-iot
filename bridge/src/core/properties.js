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

module.exports = {getDefaultValue, validate}
