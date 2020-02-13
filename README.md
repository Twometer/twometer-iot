# Twometer IoT System

### Definitions
- Bridge: ESP8266 Controller hosting the command network
- Device: A smart device connected to the command network
- Controller: User-operated device used to send commands (e.g. App)
- Property: All devices have properties, for example color or light intensity

### Protocol
Communication is based on a simple REST protocol. Each device hosts a server on port 80 that allows the bridge to control it. The bridge has such a server running as well, but with a different set of endpoints. This server can be used to control the smart network from the outside world.

Configuration of the bridge etc. is done using the Android App.

#### General responses
```json
{
    "status": "ok|bad_request|forbidden|error"
}
```

Endpoints listed below that don't explicitly specify a response use this default response.


### Devices

#### Device Descriptor
```json
{
    "uuid": "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx",
    "type": "lights/stripe",
    "name": "GlowTec LED Stripe",
    "manufacturer": "GlowTec Industries"
}
```

#### Allowed device types
- `lights/generic`: A generic single-bulb light
- `lights/stripe`: A light stripe
- `switch/generic`: A generic on-off switch
- `switch/multi`: A switch with multiple outputs
- `button/generic`: A generic momentary button
- `button/multi`: A momentary button with multiple outputs

#### Allowed data types
- `int`: Single integer value
- `bool`: Single boolean value
- `float`: Single floating-point value
- `string`: Text value
- `color/rgb`: RGB color
- `color/rgbw`: RGBW color
- `color/rgb2w`: RGBWWCW color
- `custom`: A custom set of values

#### Endpoints

**`GET /`**
Returns the device descriptor, see above.

**`GET /ping`**
Used for testing if the device is online. The bridge calls this endpoint regularly to update the list of connected devices.

```json
{
    "status": "ok"
}
```

**`GET /prop`**
Gets a list of properties.

```json
[{
	"name": "color",
	"type": "color/rgbw"
}, ...] 
```

**`PUT /{prop}`**
Used for updating the given property

```json
{
    "r": 255, "g": 0, "b": 0, "w": 128
}
```

### Bridge
The bridge has two modes, pairing mode and regular mode. Pairing mode is entered by pressing the pair button on the bridge. The bridge will then shut down the control WiFi and instead host an unencrypted pairing WiFi. If no device or controller connects to this WiFi within 3 minutes, pairing mode exits. Once a device connects, it can download the credentials for the control WiFi. Once a request to the credentials download endpoint happens, pairing mode will immediately exit. The device can then connect to the control WiFi and register using `/register`

#### Pairing Mode

**`GET /keys`**
Gets the key required to connect to the control WiFi as well as the token required to register. Only active during pairing mode.

```json
{
    "key": "",
    "token": ""
}
```

#### Regular Mode

**``POST /login``**
Notifies the bridge of a device that has come back online.

```json
{
    "uuid": ""
}
```

**`POST /register`**
Registers a new device or controller with the bridge. The token in the request must match the token issued during pairing. For controllers, the bridge returns an access token that has to be passed in the Authorization Header. With this token, it can send commands to the bridge.

```json
{
    "token": "",
    "type": "controller|device"
}
```

```json
{
    "status": "ok|forbidden",
    "token": ""
}
```

**`GET /`**
Returns general public information about the bridge so that `/` does not result in a 404. Makes it easier to identify the device as an IoT bridge.

```json
{
    "name": "Twometer IoT Bridge",
    "version": "1.0.0"
}
```

#### Available REST Endpoints
> Note: Endpoints requiring authorization are marked with 🔒, public ones with 🌐

##### Commands
- 🌐 `POST /commands?name={command-name}`: Executes the given command
- 🌐 `GET /commands`: Get a list of commands
- 🔒 `PUT /commands`: Add a new command
  ```json
  {
      "name": "",
      "actions": [
          { "device": "device-id", "properties": [] },
          { "group": "group-id", "properties": [] }
      ]
  }
  ```
- 🔒 `DELETE /commands?name={command-name}`: Delete the given command

##### Devices
- 🌐 `GET /devices`: Get a list of devices
- 🌐 `GET /devices?id={device-id}`: Get a list of properties of the given device
- 🌐 `POST /devices?id={device-id}&prop={property-name}`: Update a property of a given device
  ```json
  {
      "r": 255, "g": 0, "b": 0, "w": 128
  }
  ```
- 🔒 `DELETE /devices?id={device-id}`: Kick the given device off the network. Also invalidates its tokens.

##### Groups
- 🔒 `GET /groups`: Get a list of groups
- 🔒 `GET /groups?id={group-id}`: Get information about the group
- 🔒 `PUT /groups`: Create a new group
  ```json
  {
      "name": "",
      "devices": []
  }
  ```
  
### SDK
A simple yet powerful C++ SDK is provided, which allows easy creation of compatible IoT devices.

#### Example
```cpp
#include <TwometerIoT.h>

// Should be unique and random
const String DEVICE_ID = "55ef1248-6460-4a33-a54b-1faa0d929e7e";

TwometerIoT iot;

void setup() {
    // First the id, the name, then the type and finally the manufacturer
    iot.describe({DEVICE_ID, "GlowTec LED Stripe", TYPE_LIGHT_STRIPE, "GlowTec Industries"});
    
    iot.prop("color", DATA_COLOR_RGB)
       .handle([](const Request &req) {
           ColorRgb rgb = req.colorRgbVal();          
           // TODO Update rgb outputs
           
           return true; // Return false if request not valid
       });
        
    iot.begin();
}

void loop() {
    iot.update();
}
```

More examples can be found in the repository

##### `Request` API
- `int intVal()`: Gets a single int value
- `bool boolVal()`: Gets a single bool value
- `float floatVal()`: Gets a single float value
- `String stringVal()`: Gets a single string value
- `ColorRgb colorRgbVal()`: Gets a R, G and B value in a color struct
- `ColorRgbw colorRgbwVal()`: Gets a R, G, B and W value in a color struct
- `ColorRgb2w colorRgb2wVal()`: Gets a R, G, B, CW, and WW value in a color struct
- `JsonVariant customVal(String key)`: Gets a custom key from the request
1
To use the SDK, the ArduinoJson library is required.
