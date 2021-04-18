# Twometer's IoT system

# ✨3.x Development Branch



## Repository

- AlexaSkill: Contains the python code for the AWS lambda hosting the skill
- Bridge: Contains the C++ code for the ESP8266 hosting the IoT bridge
- Devices: Contains various devices I made for the IoT system (C++/ESP)
- Library: Client library to facilitate device building (C++/ESP)
- ~~RelayClient: Client for the RelayServer, translates between Alexa and the bridge~~
- ~~RelayServer: Node between AlexaSkill and RelayClient, to avoid port forwarding~~
- ~~TwometerIotApp: Android app for managing the bridge and devices (on hold)~~



## Rebuild Roadmap

5 hops for turning on a single LED is not exactly efficient, so lets fix that and make it 4

Alexa => Skill (AWS) => Fiber (Server) => Bridge (RPi) => Device (ESP8266)