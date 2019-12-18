# Twometer IoT System

>  Note: The security of this protocol depends on the trustworthiness of the containing WiFi

## Protocol

Communication is done using a REST protocol. Every device hosts a server on port 80 that allows the bridge to control the device. The bridge also has a server running on port 80 available from the outside WiFi that can be used to control the bridge. It has a REST API for programmatic access, but it also comes with a Web UI (internally using the API), on which you can see which devices are connected, and kick or control them if neccessary. Here, one can also configure the association with Alexa and manage the commands, scenes, rooms, etc.

To let the bridge know about the capabilities of a certain device, there is a JSON file hosted on the device's server that describes endpoints, their capabilities and their possible values. The details of this communication are laid out in the following technical document.

### Device server

GET `/`
Returns general information about the device

GET `/ping`
Returns a pong

GET `/capabilities`
Returns an array of capabilities

PUT `/{capability}`
{}
Updates the capability

#### Example 1
GET `/`

```json
{ "name": "led_strip", "manufacturer": "Twometer Engineering" }
```



GET `/capabilities`

```json
[{
	"name": "color",
	"props": [ {"name": "r", "type": "int", "min": 0, "max": 255}, ... ]
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
GET `/capabilities`

```json
[{
	"name": "state",
	"props": [ {"type": "enum", "values": ["cinema", "bluetooth"]} ]
}, ...]
```



PUT `/state`

```json
{ "state": "cinema" }
```

### Bridge Server

The bridge calls each device's `/ping` endpoint regularly (~5 minutes) to update the list of actually connected devices.

#### Pairing mode

Pairing mode is entered by pressing the pair button on the bridge. The bridge will then shut down the control WiFi and instead host an unencrypted pairing WiFi. If no device connects to this WiFi within 3 minutes, pairing mode exits. Once a device connects, it can download the credentials for the control WiFi. Once a request to the credentials download endpoint happens, pairing mode will immediately exit. The device can then connect to the control WiFi and register using `/register`

GET `/credentials`

```json
{ "key": "x" }
```

Returns the key that is required to connect to the control WiFi. This endpoint is only active during pairing mode, and a request to this endpoints causes pairing mode to exit.



#### Regular mode

POST `/register`

Registers a device with the bridge. The bridge can reject the request, for example if the device in question is already registered (e.g. when a lightbulb tries to register again as a power socket, that doesn't seem valid). Registration is only required to be done after pairing.

> Note: Only if this request is issued, the device appears in the configuration panel and can be used by the user.



POST `/admin`

```json
{"password": ""}
```

```
{"status": "ok", "token": "~"}
```

The authentication token should be included in the Authentication header thereafter.  The scheme should be `Token`. This is only required for **admin** tasks.



**Available REST endpoints:**

> Note: Admin-only endpoints are marked with ⚠️, public ones with ✔️

✔️ GET `/commands`

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