package de.twometer.iot.handler;

import org.json.JSONObject;

public class Request {

    private String action;
    private String endpoint;
    private String instance;
    private String correlationToken;
    private JSONObject payload;

    public Request(String action, String endpoint, String instance, String correlationToken, JSONObject payload) {
        this.action = action;
        this.endpoint = endpoint;
        this.instance = instance;
        this.correlationToken = correlationToken;
        this.payload = payload;
    }

    public String getAction() {
        return action;
    }

    public String getEndpoint() {
        return endpoint;
    }

    public String getInstance() {
        return instance;
    }

    public String getCorrelationToken() {
        return correlationToken;
    }

    public JSONObject getPayload() {
        return payload;
    }

}
