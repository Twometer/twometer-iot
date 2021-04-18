# Legacy API

This page describes the legacy version 2 API. Version 3 servers must be backwards-compatible with version 2 devices. 

Note that version 1 and 2 were called "Twometer IoT" instead of "Luminosity". Therefore, some legacy documentation and/or code may still use the o

## Protocol

Protocol: REST (HTTP)
Request content type: application/json

Bridge states:

 - Pairing: Opens /keys endpoint and pairing WiFi to distribute key material
 - Regular: Regular operation

## Bridge endpoints

```
GET /
 Returns version and name

GET /debug
 Returns debug information

GET /keys
 Returns key and token

POST /login
 Send "uuid" to signal the device is online

POST /register
 Send "uuid", "name", "type", "manufacturer", "token" to pair with the bridge.
 Token is the token from /keys

POST /name
 Query params: "id" (Device id), "name" (Display name)
 Sets a display name for a device (Such as "Bedroom lights" instead of
 "HUE_LED_STRIP_5M_3912")

POST /set
 Query params: "id" (Device id), "prop" (Property to set)
 Send property parameters as body
 Set the property "prop" of the device "id" to the value supplied in body

GET /prop
 Query params: "id" (Device id)
 Get a list of properties of a device along with the data types

GET /devices
 Get a list of devices, consisting of each a "uuid", "name", "type", "manufacturer",
 and if set, "friendly_name"
```

## Device endpoints

```
GET /
 Returns device descriptor (uuid, name, type, manufacturer)

GET /ping
 Returns a status ok message

GET /prop
Returns a list of properties and their data types

PUT /{prop}
Sets the value of the property to the value supplied in body formatted
as JSON.
```

## Discover

To discover a bridge in the network, send a UDP broadcast to your subnet (e.g. 192.168.1.255)
on port 38711 consisting of the bytes "0x00 0x42 0x69". The bridge will reply to you with its
IP address formatted as an ASCII string (e.g. 192.168.1.123).


## Alexa Integration

Alexa integration is done using the relay and the AlexaSkill. Communication is as following:

Alexa => AlexaSkill (AWS Lambda) => RelayServer (Dedicated Server) => RelayClient (Translator
 within IoT network) => Bridge (ESP8266) => Device (ESP8266)

Reasons for this complicated stuff:

- The AWS lambda is required because Alexa skills REQUIRE an AWS Lambda as backend
- The RelayServer is required to avoid port forwarding in the local network
- The RelayClient is required because I can't run it directly on the ESP8266 because it does
  not have enough cores for a server and a client (single-core system)