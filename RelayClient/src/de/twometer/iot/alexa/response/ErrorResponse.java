package de.twometer.iot.alexa.response;

import de.twometer.iot.alexa.model.ErrorType;
import de.twometer.iot.handler.Request;
import org.json.JSONObject;

import java.util.UUID;

import static de.twometer.iot.json.JSONStatic.newObject;

public class ErrorResponse implements IResponse {

    private String correlationToken;
    private String endpointId;
    private ErrorType errorType;
    private String errorMessage;

    public ErrorResponse(Request request, ErrorType errorType, String errorMessage) {
        this.correlationToken = request.getCorrelationToken();
        this.endpointId = request.getEndpoint();
        this.errorType = errorType;
        this.errorMessage = errorMessage;
    }

    @Override
    public JSONObject toJson() {
        return newObject().put("event", newObject()
                .put("header", newObject().put("namespace", "Alexa").put("name", "ErrorResponse").put("payloadVersion", "3").put("messageId", UUID.randomUUID().toString()).put("correlationToken", correlationToken))
                .put("endpoint", newObject().put("endpointId", endpointId))
                .put("payload", newObject().put("type", errorType.name()).put("message", errorMessage))
        ).object();
    }
}
