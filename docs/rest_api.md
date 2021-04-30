# REST API

This page describes the modern REST API to interact with the bridge.

Endpoints that do not have a response use HTTP status codes to inform the caller about the status.

## Status
```
GET /
```
```json
{
    "name": "lumi-bridge",
    "version": "3.0.0",
    "devices": 42,
    "status": "online"
}
```

## Admin

```json
POST /admin
{
    "action": "enable_pairing|..."
}
```

## Pair

```json
POST /pair
{
    "device": device_descriptor
}
```

```json
{
    "wifiKey": "",
    "authToken": ""
}
```

## List Devices

```
GET /device
```



## List Scenes

```
GET /scene
```

