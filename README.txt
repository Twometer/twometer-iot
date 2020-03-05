Twometer's IoT system

Protocol:
=========
Protocol: REST (HTTP)
Request content type: application/json

Bridge states:
- Pairing: Opens /keys endpoint and pairing WiFi to distribute key material
- Regular: Regular operation

Bridge endpoints:
-----------------
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
Set a display name for a device (Such as "Lights Bedroom" instead of "HUE_LED_STRIP_5M_3912")

POST /set
Query params: "id" (Device id), "prop" (Property to set)
Send property parameters as body
Set the property "prop" of the device "id" to the value supplied in body

GET /prop
Query params: "id" (Device id)
Get a list of properties of a device along with the data types

GET /devices
Get a list of devices, consisting of each a "uuid", "name", "type", "manufacturer", and if set, "friendly_name"

Device endpoints:
-----------------
GET /
Returns device descriptor (uuid, name, type, manufacturer)

GET /ping
Returns a status ok message

GET /prop
Returns a list of properties and their data types

PUT /{prop}
Sets the value of the property to the value supplied in body formatted
as JSON.
