# Twometer IoT System

## Protocol

Communication is done using a REST protocol. Every device hosts a server on port 80 that allows the bridge to control the device. The bridge also has a server running on port 80 available from the outside WiFi that can be used to control the bridge. It has a REST API for programmatic access, but it also comes with a Web UI (internally using the API), on which you can see which devices are connected, and kick or control them if neccessary. Here, one can also configure the association with Alexa and manage the commands, scenes, rooms, etc.

To let the bridge know about the capabilities of a certain device, there is a JSON file hosted on the device's server that describes endpoints, their capabilities and their possible values. The details of this communication is laid out in the following technical document.