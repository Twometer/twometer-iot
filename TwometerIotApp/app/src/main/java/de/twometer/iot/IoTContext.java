package de.twometer.iot;

import android.content.Context;

import java.io.IOException;

import de.twometer.iot.model.Bridge;
import de.twometer.iot.net.api.ApiClient;
import de.twometer.iot.net.discovery.BridgeDiscovery;

public class IoTContext {

    private Bridge bridge;

    private ApiClient client;

    public void initialize(Context context) {
        try {
            bridge = new Bridge(BridgeDiscovery.discover(context));
            client = new ApiClient(bridge);
            client.loadBridgeInfo();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
