# ✨Luminosity IoT system

Luminosity is a DIY open source IoT and smart home system.

> **This is the version 3 development branch**



## Repository layout

- bridge: Node.js application designed for Raspberry Pi which hosts the IoT bridge
- devices: Contains a collection of IoT device implementations
- docs: The mkdocs documentation for the project
- lambda: The AWS lambda which connects the bridge to Alexa.
- library: Arduino library for easy building of client devices.


## Data flow
`Alexa` -> `AWS Lambda` -> `fibers` -> `Bridge (RPi)` -> `Device (ESP8266)`

Fibers is my [message passing service](https://github.com/Twometer/fibers)

The bridge can be controlled and configured using clients in the same local network, without any use of Amazon components.
