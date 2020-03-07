package de.twometer.iot.handler;

import de.twometer.iot.alexa.ErrorResponse;
import de.twometer.iot.alexa.ErrorType;
import de.twometer.iot.alexa.IResponse;
import de.twometer.iot.alexa.StateUpdateResponse;
import de.twometer.iot.model.values.SimpleValue;
import de.twometer.iot.net.BridgeClient;

public class BrightnessHandler implements IHandler {

    @Override
    public IResponse handle(Request request, BridgeClient client) {
        int brightness = request.getPayload().getInt("brightness");
        boolean ok = client.setProperty(request.getEndpoint(), "Lamp.Brightness", new SimpleValue<>("brightness", brightness));
        if (ok) return new StateUpdateResponse(request.getCorrelationToken(), getNamespace(), "brightness", brightness);
        else
            return new ErrorResponse(request.getCorrelationToken(), request.getEndpoint(), ErrorType.ENDPOINT_UNREACHABLE, "Failed to set value on endpoint");
    }

    @Override
    public String getNamespace() {
        return "Alexa.BrightnessController";
    }

}
