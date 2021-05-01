# UDP protocol

Communication between the bridge and the device happens with a lightweight UDP protocol.

The protocol is comprised of simple text messages of different types.

## Message format

The message format is very basic:

```plaintext
<PacketType>:<Arg1>:<Arg2>:...
```



## Message types

- `DISCOV`: Broadcast, and the bridge will reply with `BHELLO` and its IP address
- `BHELLO`: Reply to `DISCOV`, contains the IP address in arg1
- `DHELLO`: Device hello to notify the bridge that a device is online
- `UPDATE`: Sent from the bridge to the device to update a property
- `REPORT`: Sent from the device to the bridge to report a value has changed (e.g. a sensor that updated)
- `PING`: Keepalive from the bridge to the device. 
- `PONG`: Device's response to `PING`