package de.twometer.iot.model.values;

import org.json.JSONException;
import org.json.JSONObject;

public class Rgb2wValue implements Value {

    private int r;
    private int g;
    private int b;
    private int cw;
    private int ww;

    public Rgb2wValue(int r, int g, int b, int cw, int ww) {
        this.r = r;
        this.g = g;
        this.b = b;
        this.cw = cw;
        this.ww = ww;
    }

    @Override
    public JSONObject toJsonObject() {
        try {
            JSONObject object = new JSONObject();
            object.put("r", r);
            object.put("g", g);
            object.put("b", b);
            object.put("cw", cw);
            object.put("ww", ww);
            return object;
        } catch (JSONException e) {
            e.printStackTrace();
            return null;
        }
    }
}
