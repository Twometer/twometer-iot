package de.twometer.iot.model.values;

import org.json.JSONException;
import org.json.JSONObject;

public class RgbValue implements Value {

    private int r;
    private int g;
    private int b;

    public RgbValue(int r, int g, int b) {
        this.r = r;
        this.g = g;
        this.b = b;
    }

    @Override
    public JSONObject toJsonObject() {
        try {
            JSONObject object = new JSONObject();
            object.put("r", r);
            object.put("g", g);
            object.put("b", b);
            return object;
        } catch (JSONException e) {
            e.printStackTrace();
            return null;
        }
    }
}
