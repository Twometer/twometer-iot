package de.twometer.iot.rest;

import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.IOException;

public class HttpRequest {

    private final String body;

    private final String query;

    public HttpRequest(String body, String query) {
        this.body = body;
        this.query = query;
    }

    public String getQuery() {
        return query;
    }

    public String getBody() {
        return body;
    }

    public <T> T getBody(Class<T> clazz) {
        ObjectMapper mapper = new ObjectMapper();
        try {
            return mapper.readValue(body, clazz);
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

}
