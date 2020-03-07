package de.twometer.iot.handler;

import de.twometer.iot.alexa.model.ErrorType;
import de.twometer.iot.alexa.response.ErrorResponse;
import de.twometer.iot.alexa.response.IResponse;
import de.twometer.iot.alexa.response.StateResponse;
import de.twometer.iot.bridge.values.SimpleValue;
import de.twometer.iot.handler.base.IHandler;
import de.twometer.iot.net.BridgeClient;

public class PowerHandler implements IHandler {

    @Override
    public IResponse handle(Request request, BridgeClient client) {
        boolean newState = request.getAction().equals("TurnOn");
        boolean ok = client.setProperty(request.getEndpoint(), "Device.PowerState", new SimpleValue<>("powerState", newState));

        if (ok)
            return new StateResponse(request.getCorrelationToken(), getNamespace(), "powerState", newState);
        else
            return new ErrorResponse(request.getCorrelationToken(), request.getEndpoint(), ErrorType.ENDPOINT_UNREACHABLE, "Failed to set value on endpoint");
    }

    @Override
    public String getNamespace() {
        return "Alexa.PowerController";
    }

}
