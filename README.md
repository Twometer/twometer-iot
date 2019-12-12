# Twometer IoT System

## Protocol

Communication is done using a REST protocol. Every device hosts a server on port 80 that allows the bridge to control the device. The bridge also has a server running on port 80 available from the outside WiFi that can be used to control the bridge. It has a REST API for programmatic access, but it also comes with a Web UI (internally using the API), on which you can see which devices are connected, and kick or control them if neccessary. Here, one can also configure the association with Alexa and manage the commands, scenes, rooms, etc.

To let the bridge know about the capabilities of a certain device, there is a JSON file hosted on the device's server that describes endpoints, their capabilities and their possible values. The details of this communication are laid out in the following technical document.

### Device server

GET /deviceinfo
Returns information about the device

GET /capabilities
Returns an array of capabilities

PUT /{capability}
{}
Updates the capability

#### Example 1:
GET /deviceinfo

```json
{ "name": "led_strip", "manufacturer": "Twometer Engineering" }
```



GET /capabilities

```json
[{
	"name": "color",
	"props": [ {"name": "r", "type": "int", "min": 0, "max": 255}, ... ]
}, ...] 
```



PUT /color

```json
{
	"r": 0,
	"g": 12,
	"b": 255,
	"w": 15
}
```



#### Example 2:
GET /capabilities

```json
[{
	"name": "state",
	"props": [ {"type": "enum", "values": ["cinema", "bluetooth"]} ]
}, ...]
```



PUT /state

```json
{ "state": "cinema" }
```

### Bridge Server

#### Pairing mode
GET /credentials

```json
{ "key": "x" }
```

Returns the key that is required to connect to the control WiFi.



#### Regular mode

POST /login

```json
{"password": ""}
```

```
{"status": "ok", "session": "~"}
```

The session token should be included in the Authentication header thereafter.  The scheme should be `Session`. This is only required for **admin** tasks.





GET /commands

PUT /commands/{command-name}



GET /rooms

PUT /rooms/room-name



GET /devices

PUT /devices/{device-id}/{capability}

DELETE /devices/{device-id}