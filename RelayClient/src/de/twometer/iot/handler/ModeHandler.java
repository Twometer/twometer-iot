package de.twometer.iot.handler;

import de.twometer.iot.alexa.ErrorResponse;
import de.twometer.iot.alexa.ErrorType;
import de.twometer.iot.alexa.IResponse;
import de.twometer.iot.alexa.StateUpdateResponse;
import de.twometer.iot.model.values.SimpleValue;
import de.twometer.iot.net.BridgeClient;

public class ModeHandler implements IHandler {

    @Override
    public IResponse handle(Request request, BridgeClient client) {
        String mode = request.getPayload().getString("mode");
        boolean ok = client.setProperty(request.getEndpoint(), request.getInstance(), new SimpleValue<>("mode", mode));

        if (ok)
            return new StateUpdateResponse(request.getCorrelationToken(), getNamespace(), request.getInstance(), mode);
        else
            return new ErrorResponse(request.getCorrelationToken(), request.getEndpoint(), ErrorType.ENDPOINT_UNREACHABLE, "Failed to set value on endpoint");
    }

    @Override
    public String getNamespace() {
        return "Alexa.ModeController";
    }

}
