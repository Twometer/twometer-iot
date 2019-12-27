# Twometer IoT System

>  Note: The security of this protocol depends on the trustworthiness of the containing WiFi

## Protocol

Communication is done using a REST protocol. Every device hosts a server on port 80 that allows the bridge to control the device. The bridge also has a server running on port 80 available from the outside WiFi that can be used to control the bridge. It has a REST API for programmatic access, but it also comes with a Web UI (internally using the API), on which you can see which devices are connected, and kick or control them if neccessary. Here, one can also configure the association with Alexa and manage the commands, scenes, rooms, etc.

To let the bridge know about the capabilities of a certain device, there is a JSON file hosted on the device's server that describes endpoints, their capabilities and their possible values. The details of this communication are laid out in the following technical document.

### Device server

GET `/`
Returns general information about the device, such as name, type and manufacturer

GET `/ping`
Returns a pong: `{"state": "ok"}`

GET `/prop`
Returns an array of properties

PUT `/{prop}`
{...}
Updates the property

#### Device types

Device type identifiers consist of two parts, and are formatted `{group}/{device}`. The complete list can be found in `Library/Constants.h`.

#### Example 1
GET `/`

```json
{ "name": "led_strip", "type": "lights/stripe", "manufacturer": "Twometer Engineering" }
```



GET `/prop`

```json
[{
	"name": "color",
	"values": [ {"name": "r", "type": "int", "min": 0, "max": 255}, ... ]
}, ...] 
```



PUT `/color`

```json
{
	"r": 0,
	"g": 12,
	"b": 255,
	"w": 15
}
```



#### Example 2
GET `/prop`

```json
[{
	"name": "state",
	"values": [ {"name": "value", "type": "enum", "values": ["cinema", "bluetooth"]} ]
}, ...]
```



PUT `/state`

```json
{ "value": "cinema" }
```

### Bridge Server

The bridge calls each device's `/ping` endpoint regularly (~5 minutes) to update the list of actually connected devices.

#### Pairing mode

Pairing mode is entered by pressing the pair button on the bridge. The bridge will then shut down the control WiFi and instead host an unencrypted pairing WiFi. If no device connects to this WiFi within 3 minutes, pairing mode exits. Once a device connects, it can download the credentials for the control WiFi. Once a request to the credentials download endpoint happens, pairing mode will immediately exit. The device can then connect to the control WiFi and register using `/register`

GET `/credentials`

```json
{ "key": "x", "token": "y" }
```

Returns the key that is required to connect to the control WiFi, as well as the registration token. This endpoint is only active during pairing mode, and a request to this endpoint causes pairing mode to exit.



#### Regular mode

POST `/register`

```json
{ "token": "y" }
```

Registers a device with the bridge. The bridge only accepts tokens issued during pairing mode.

> Note: Only if this request is issued, the device appears in the configuration panel and can therefore be used by the user.



POST `/admin`

```json
{"password": ""}
```

```
{"status": "ok", "token": "~"}
```

The authentication token should be included in the Authentication header thereafter.  The scheme should be `Token`. This is only required for **admin** tasks.



**Available REST endpoints:**

>  🚧 This section is still under construction 🚧



> Note: Admin-only endpoints are marked with ⚠️, public ones with ✔️

✔️ POST `/{command-name}`

⚠️ GET `/commands`

⚠️ PUT `/commands/{command-name}`

⚠️ DELETE `/commands/{command-name}`



✔️ GET `/rooms`

⚠️ PUT `/rooms/{room-name}`

⚠️ DELETE `/rooms/{room-name}`



✔️ GET `/devices`

✔️ PUT `/devices/{device-id}/{capability}`

⚠️ DELETE `/devices/{device-id}`





## Web Interface

The bridge hosts a web console on port 80 under `/` . Internally, it uses the REST API to communicate with the bridge. It requires administrative login, and it has the following features:

- See all registered (paired) devices, the last time an action was executed, and the last time they were online

  - Kick devices off the network (Unpair)
  - Rename devices (aliases)
  - Send raw API requests (developer mode)

- Configure commands for Alexa such as "Alexa, good night", and which actions to execute

- Configure rooms

- Change admin credentials

- View and regenerate encryption keys

  >  Note: Key regeneration is always done after a device was kicked



## SDK

>  🚧 This section is still under construction, the API might change in the future 🚧

An easy-to-use yet powerful C++ programming interface is provided, which allows makers to easily make their device Twometer IoT compatible.

### Example 1

```c++
#include <TwometerIoT.h>

TwometerIoT iot;

void setup() {
    // First the name, then the type and finally the manufacturer
    iot.describe({"GlowTec LED Strip", TYPE_LIGHT_STRIPE, "GlowTec Industries"});
    
    iot.prop("color")
       .intVal("r", 0, 255)
       .intVal("g", 0, 255)
       .intVal("b", 0, 255)
       .handle([](const Request &req) {
           int r = req.intVal("r");
           int g = req.intVal("g");
           int b = req.intVal("b");
           
           // TODO Update rgb outputs
       });
        
    iot.begin();
}

void loop() {
    iot.update();
}
```



### Example 2

```cpp
#include <TwometerIoT.h>

TwometerIoT iot;

void setup() {
    iot.prop("state")
       .enumVal("state", { "cinema", "bluetooth" })
       .handle([](const Request &req) {
          String &val = req.enumVal();
          
          // TODO switch outputs
       });
    
    iot.begin();
}

void loop() {
    iot.update();
}
```

