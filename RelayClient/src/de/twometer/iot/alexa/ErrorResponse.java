package de.twometer.iot.alexa;

import org.json.JSONObject;

import java.util.UUID;

import static de.twometer.iot.json.JSONStatic.newObject;

public class ErrorResponse implements IResponse {

    private String endpointId;
    private ErrorType errorType;
    private String errorMessage;

    public ErrorResponse(String endpointId, ErrorType errorType, String errorMessage) {
        this.endpointId = endpointId;
        this.errorType = errorType;
        this.errorMessage = errorMessage;
    }

    @Override
    public JSONObject toJson() {
        return newObject().put("event", newObject()
                .put("header", newObject().put("namespace", "Alexa").put("name", "ErrorResponse").put("payloadVersion", "3").put("messageId", UUID.randomUUID().toString()))
                .put("endpoint", newObject().put("endpointId", endpointId))
                .put("payload", newObject().put("type", errorType.name()).put("message", errorMessage))
        ).object();
    }
}
