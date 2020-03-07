package de.twometer.iot.alexa;

import org.json.JSONArray;
import org.json.JSONObject;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.TimeZone;

import static de.twometer.iot.json.JSONStatic.newObject;

public class StateUpdateResponse implements IResponse {

    private String correlationToken;
    private String eventName;

    private List<PropertyItem> itemList = new ArrayList<>();

    public StateUpdateResponse(String correlationToken, String namespace, String propertyName, Object propertyValue) {
        this.correlationToken = correlationToken;
        this.eventName = "Response"; // Default event name
        itemList.add(new PropertyItem(namespace, propertyName, propertyValue));
    }

    public StateUpdateResponse(String eventName, List<PropertyItem> itemList) {
        this.eventName = eventName;
        this.itemList = itemList;
    }

    private String getDateTime() {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss'Z'");
        sdf.setTimeZone(TimeZone.getTimeZone("GMT"));
        return sdf.format(new Date());
    }

    @Override
    public JSONObject toJson() {
        JSONObject responseTemplate = new GenericResponse(correlationToken, "Alexa", eventName, new JSONObject()).toJson();

        JSONArray array = new JSONArray();
        for (PropertyItem item : itemList) {
            JSONObject object = new JSONObject();
            object.put("namespace", item.namespace);
            object.put("name", item.name);
            object.put("timeOfSample", getDateTime());
            object.put("uncertaintyInMilliseconds", 100);
            object.put("value", item.value);
            array.put(object);
        }

        responseTemplate.put("context", newObject().put("properties", array).object());

        return responseTemplate;
    }

    public static class PropertyItem {

        private String namespace;
        private String name;
        private Object value;

        public PropertyItem(String namespace, String name, Object value) {
            this.namespace = namespace;
            this.name = name;
            this.value = value;
        }
    }

}
