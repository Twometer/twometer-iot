package de.twometer.iot.handler;

import de.twometer.iot.net.BridgeClient;
import org.json.JSONObject;

public interface IHandler {

    String handle(String action, JSONObject payload, BridgeClient client);

    String getNamespace();

}
