package de.twometer.iot.model.values;

import org.json.JSONException;
import org.json.JSONObject;

public class SimpleValue<T> implements Value {

    private T t;

    public SimpleValue(T t) {
        this.t = t;
    }

    @Override
    public JSONObject toJsonObject() {
        JSONObject object = new JSONObject();
        try {
            object.put("value", t);
        } catch (JSONException e) {
            e.printStackTrace();
            return null;
        }
        return object;
    }

}
