package de.twometer.iot.handler;

import de.twometer.iot.alexa.model.ErrorType;
import de.twometer.iot.alexa.response.ErrorResponse;
import de.twometer.iot.alexa.response.IResponse;
import de.twometer.iot.alexa.response.StateResponse;
import de.twometer.iot.bridge.values.JsonValue;
import de.twometer.iot.net.BridgeClient;
import org.json.JSONObject;

public class ColorHandler implements IHandler {

    @Override
    public IResponse handle(Request request, BridgeClient client) {
        JSONObject bridgePayload = request.getPayload().getJSONObject("color");
        boolean ok = client.setProperty(request.getEndpoint(), "Lamp.Color", new JsonValue(bridgePayload));
        if (ok) return new StateResponse(request.getCorrelationToken(), getNamespace(), "color", bridgePayload);
        else
            return new ErrorResponse(request.getCorrelationToken(), request.getEndpoint(), ErrorType.ENDPOINT_UNREACHABLE, "Failed to set value on endpoint");
    }

    @Override
    public String getNamespace() {
        return "Alexa.ColorController";
    }
}
