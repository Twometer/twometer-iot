package de.twometer.iot.alexa;

import org.json.JSONObject;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;

import static de.twometer.iot.json.JSONStatic.newArray;
import static de.twometer.iot.json.JSONStatic.newObject;

public class StateUpdateResponse implements IResponse {

    private String namespace;
    private String propertyName;
    private Object propertyValue;

    public StateUpdateResponse(String namespace, String propertyName, Object propertyValue) {
        this.namespace = namespace;
        this.propertyName = propertyName;
        this.propertyValue = propertyValue;
    }

    private String getDateTime() {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss'Z'");
        sdf.setTimeZone(TimeZone.getTimeZone("GMT"));
        return sdf.format(new Date());
    }

    @Override
    public JSONObject toJson() {
        JSONObject responseTemplate = new GenericResponse("Alexa", "Response", new JSONObject()).toJson();

        responseTemplate.put("context", newObject().put("properties", newArray().put(newObject()
                .put("namespace", namespace)
                .put("name", propertyName)
                .put("timeOfSample", getDateTime())
                .put("uncertaintyInMilliseconds", 100)
                .put("value", propertyValue)
        )).object());

        return responseTemplate;
    }

}
