package de.twometer.iot.handler;

import org.json.JSONObject;

public class Request {

    private String action;
    private String endpoint;
    private String correlationToken;
    private JSONObject payload;

    public Request(String action, String endpoint, String correlationToken, JSONObject payload) {
        this.action = action;
        this.endpoint = endpoint;
        this.correlationToken = correlationToken;
        this.payload = payload;
    }

    public String getAction() {
        return action;
    }

    public String getEndpoint() {
        return endpoint;
    }

    public String getCorrelationToken() {
        return correlationToken;
    }

    public JSONObject getPayload() {
        return payload;
    }

}
