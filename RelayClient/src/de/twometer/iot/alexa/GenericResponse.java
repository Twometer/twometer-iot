package de.twometer.iot.alexa;

import org.json.JSONObject;

import java.util.UUID;

import static de.twometer.iot.json.JSONStatic.newObject;

public class GenericResponse implements IResponse {

    private final String messageId = UUID.randomUUID().toString();
    private String namespace;
    private String name;
    private JSONObject payload;

    public GenericResponse(String namespace, String name, JSONObject payload) {
        this.namespace = namespace;
        this.name = name;
        this.payload = payload;
    }

    public JSONObject toJson() {
        final String payloadVersion = "3";

        return newObject().put("event",
                newObject().put("header",
                        newObject().put("namespace", namespace)
                                .put("name", name)
                                .put("payloadVersion", payloadVersion)
                                .put("messageId", messageId)
                ).put("payload", payload)
        ).object();
    }

}
