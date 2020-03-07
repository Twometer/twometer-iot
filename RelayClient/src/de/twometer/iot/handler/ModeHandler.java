package de.twometer.iot.handler;

import de.twometer.iot.alexa.ErrorResponse;
import de.twometer.iot.alexa.ErrorType;
import de.twometer.iot.alexa.IResponse;
import de.twometer.iot.alexa.StateUpdateResponse;
import de.twometer.iot.model.values.SimpleValue;
import de.twometer.iot.net.BridgeClient;
import org.json.JSONObject;

public class ModeHandler implements IHandler {

    @Override
    public IResponse handle(String action, String endpoint, JSONObject payload, BridgeClient client) {
        String mode = payload.getString("mode");
        boolean ok = client.setProperty(endpoint, "TV.InputChannel", new SimpleValue<>("mode", mode));
        if (ok) return new StateUpdateResponse(getNamespace(), mode, mode);
        else return new ErrorResponse(endpoint, ErrorType.ENDPOINT_UNREACHABLE, "Failed to set value on endpoint");
    }

    @Override
    public String getNamespace() {
        return "Alexa.ModeController";
    }

}
