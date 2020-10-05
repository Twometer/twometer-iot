package de.twometer.iot.alexa.response;

import org.json.JSONObject;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;
import java.util.UUID;

import static de.twometer.iot.json.JSONStatic.newObject;

public class SceneResponse implements IResponse {

    private final String correlationToken;
    private final String sceneId;
    private final String name;

    public SceneResponse(String correlationToken, String sceneId, String name) {
        this.correlationToken = correlationToken;
        this.sceneId = sceneId;
        this.name = name;
    }

    @Override
    public JSONObject toJson() {
        Date date = new Date(System.currentTimeMillis());
        SimpleDateFormat sdf;
        sdf = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSSXXX");
        sdf.setTimeZone(TimeZone.getTimeZone("CET"));
        String timestamp = sdf.format(date);

        return newObject().put("event", newObject()
                .put("header", newObject().put("namespace", "Alexa").put("name", name).put("payloadVersion", "3").put("messageId", UUID.randomUUID().toString()).put("correlationToken", correlationToken))
                .put("endpoint", newObject().put("endpointId", sceneId))
                .put("payload", newObject().put("cause", newObject().put("type", "VOICE_INTERACTION")).put("timestamp", timestamp))
        ).object();
    }
}
