# Pairing

To add a new device to the system, the following steps happen:

1. The admin enables pairing mode on the console
2. The bridge opens the unencrypted pairing WiFi
3. The device requests the bridge's `pair` endpoint with its device ID.
4. The bridge returns the WiFi credentials as well as an authorization token to the new device, and closes the unencrypted WiFi. The bridge now exists pairing mode.
5. The new device connects to the normal WiFi and reports its metadata and capabilities, authorized by the token. This token must be saved by the device.