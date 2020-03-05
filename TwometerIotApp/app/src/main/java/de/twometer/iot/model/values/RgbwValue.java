package de.twometer.iot.model.values;

import org.json.JSONException;
import org.json.JSONObject;

public class RgbwValue implements Value {

    private int r;
    private int g;
    private int b;
    private int w;

    public RgbwValue(int r, int g, int b, int w) {
        this.r = r;
        this.g = g;
        this.b = b;
        this.w = w;
    }

    @Override
    public JSONObject toJsonObject() {
        try {
            JSONObject object = new JSONObject();
            object.put("r", r);
            object.put("g", g);
            object.put("b", b);
            object.put("w", w);
            return object;
        } catch (JSONException e) {
            e.printStackTrace();
            return null;
        }
    }
}
