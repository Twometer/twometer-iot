package de.twometer.iot.handler;

import de.twometer.iot.net.BridgeClient;
import org.json.JSONObject;

public class DiscoveryHandler implements IHandler {

    @Override
    public String handle(String action, JSONObject payload, BridgeClient client) {
        return null;
    }

    @Override
    public String getNamespace() {
        return "Alexa.Discovery";
    }

}
