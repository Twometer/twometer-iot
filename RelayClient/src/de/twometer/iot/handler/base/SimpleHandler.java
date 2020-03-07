package de.twometer.iot.handler.base;

import de.twometer.iot.alexa.model.ErrorType;
import de.twometer.iot.alexa.response.ErrorResponse;
import de.twometer.iot.alexa.response.IResponse;
import de.twometer.iot.alexa.response.StateResponse;
import de.twometer.iot.bridge.values.JsonValue;
import de.twometer.iot.bridge.values.SimpleValue;
import de.twometer.iot.handler.Request;
import de.twometer.iot.net.BridgeClient;
import org.json.JSONObject;

public abstract class SimpleHandler implements IHandler {

    private String alexaPropertyName;
    private String bridgePropertyName;
    private ValueType valueType;

    public SimpleHandler(String alexaPropertyName, String bridgePropertyName, ValueType valueType) {
        this.alexaPropertyName = alexaPropertyName;
        this.bridgePropertyName = bridgePropertyName;
        this.valueType = valueType;
    }

    @Override
    public IResponse handle(Request request, BridgeClient client) {
        Object data = request.getPayload().get(alexaPropertyName);

        boolean ok;
        if (valueType == ValueType.JSON) {
            ok = client.setProperty(request.getEndpoint(), bridgePropertyName, new JsonValue((JSONObject) data));
        } else {
            ok = client.setProperty(request.getEndpoint(), bridgePropertyName, new SimpleValue<>(alexaPropertyName, data));
        }

        if (ok)
            return new StateResponse(request.getCorrelationToken(), getNamespace(), alexaPropertyName, data);
        else
            return new ErrorResponse(request.getCorrelationToken(), request.getEndpoint(), ErrorType.ENDPOINT_UNREACHABLE, "Failed to set value on endpoint");
    }

    public enum ValueType {
        PRIMITIVE,
        JSON
    }

}
