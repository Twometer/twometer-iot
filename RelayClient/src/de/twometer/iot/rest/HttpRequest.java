package de.twometer.iot.rest;

import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.IOException;

public class HttpRequest {

    private final String body;

    public HttpRequest(String body) {
        this.body = body;
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
