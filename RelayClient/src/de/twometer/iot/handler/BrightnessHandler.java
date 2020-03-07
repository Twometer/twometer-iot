package de.twometer.iot.handler;

import de.twometer.iot.alexa.ErrorResponse;
import de.twometer.iot.alexa.ErrorType;
import de.twometer.iot.alexa.IResponse;
import de.twometer.iot.alexa.StateUpdateResponse;
import de.twometer.iot.model.values.IntValue;
import de.twometer.iot.net.BridgeClient;
import org.json.JSONObject;

public class BrightnessHandler implements IHandler {

    @Override
    public IResponse handle(String action, String endpoint, JSONObject payload, BridgeClient client) {
        int brightness = payload.getInt("brightness");
        boolean ok = client.setProperty(endpoint, "Lamp.Brightness", new IntValue("brightness", brightness));
        if (ok) return new StateUpdateResponse(getNamespace(), "brightness", brightness);
        else return new ErrorResponse(endpoint, ErrorType.ENDPOINT_UNREACHABLE, "Failed to set value on endpoint");
    }

    @Override
    public String getNamespace() {
        return "Alexa.BrightnessController";
    }

}
