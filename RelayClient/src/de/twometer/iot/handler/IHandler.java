package de.twometer.iot.handler;

import de.twometer.iot.alexa.AlexaResponse;
import de.twometer.iot.net.BridgeClient;
import org.json.JSONObject;

public interface IHandler {

    AlexaResponse handle(String action, JSONObject payload, BridgeClient client);

    String getNamespace();

}
