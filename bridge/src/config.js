'use strict';

const ENV_PREFIX = 'LUMI_';

const Config = {
    REST_PORT: 0,
    FIBER_KEY: '',
    FIBER_URL: '',
    MONGO_URL: '',
    PING_WAVE_INTERVAL: 0,
    PAIRING_WIFI_SSID: '',
    WIFI_SSID: '',
    WIFI_PASS: '',
    UDP_PORT: 0,
    NET_IFACE: ''
};

function parseValue(string, type) {
    if (type === 'string')
        return string;
    else if (type === 'boolean')
        return type === 'true';
    else if (type === 'number')
        return parseInt(string);
    else
        throw Error(`Cannot parse config value of type ${type}`);
}

Config._loadValue = function (key, type) {
    let prefixedKey = ENV_PREFIX + key;
    let configValue = process.env[prefixedKey];
    if (configValue === undefined) {
        throw Error(`Missing environment variable '${prefixedKey}'`);
    }
    this[key] = parseValue(configValue, type);
}

Config.load = function () {
    for (let key of Object.keys(this)) {
        let valueType = typeof this[key];
        if (valueType === 'function') continue;
        this._loadValue(key, valueType);
    }
}

module.exports = Config;