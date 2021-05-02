'use strict';

const PropertyMap = [
    {
        alexaNamespace: 'Alexa.BrightnessController',
        alexaProperty: 'brightness',
        lumiProperty: 'Light.Brightness'
    },
    {
        alexaNamespace: 'Alexa.ColorTemperatureController',
        alexaProperty: 'colorTemperatureInKelvin',
        lumiProperty: 'Light.Temp'
    },
    {
        alexaNamespace: 'Alexa.ColorController',
        alexaProperty: 'color',
        lumiProperty: 'Light.Color'
    },
    {
        alexaNamespace: 'Alexa.PowerController',
        alexaProperty: 'powerState',
        lumiProperty: 'Device.PowerState'
    },
    {
        alexaNamespace: 'Alexa.PowerLevelController',
        alexaProperty: 'powerLevel',
        lumiProperty: 'Device.PowerLevel'
    },
    {
        alexaNamespace: 'Alexa.ModeController',
        alexaProperty: 'mode',
        lumiProperty: null
    }
];

module.exports = {

    getByNamespace(namespace) {
        return PropertyMap.filter(entry => entry.alexaNamespace === namespace)[0]
    },

    getByProperty(lumiName) {
        return PropertyMap.filter(entry => entry.lumiProperty === lumiName)[0]
    }
}