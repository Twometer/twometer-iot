# Devices

Devices are uniquiely identified by their Device ID. This can for example be the ESP8266 Chip ID.

Furthermore, the full device information is stored in a so-called _device descriptor_: This device descriptor looks like the following:

```json
{
    deviceId: "",
    type: "",
    modelName: "",
    friendlyName: "",
    manufacturer: "",
    description: "",
    properties: [{key: "", type: "", friendlyName: "", valueRange: ""}]
}
```

Note that `valueRange` is only accepted when the property is of type `MODE`

## Device Types

Uses the same device types that [Amazon Alexa](https://developer.amazon.com/en-US/docs/alexa/device-apis/alexa-discovery.html#display-categories) uses.

## Property types

- `EVENT`: A property that does not actually have a value. It is used to signal an event (e.g. a tactile switch was pressed)
- `BOOLEAN`: A property that can be either `true` or `false`
- `NUMBER`: A decimal number
- `COLOR`: A color with the properties `hue`, `saturation`, `brightness`
- `MODE`: A property that describes one of many modes.