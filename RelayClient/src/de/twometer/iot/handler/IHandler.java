package de.twometer.iot.handler;

import de.twometer.iot.alexa.IResponse;
import de.twometer.iot.net.BridgeClient;
import org.json.JSONObject;

public interface IHandler {

    IResponse handle(String action, String endpoint, JSONObject payload, BridgeClient client);

    String getNamespace();

}
