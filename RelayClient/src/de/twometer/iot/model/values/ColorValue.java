package de.twometer.iot.model.values;

import de.twometer.iot.model.IValue;
import org.json.JSONObject;

public class ColorValue implements IValue {

    private float h;
    private float s;
    private float b;

    public ColorValue(float h, float s, float b) {
        this.h = h;
        this.s = s;
        this.b = b;
    }

    @Override
    public JSONObject toJson() {
        JSONObject object = new JSONObject();
        object.put("hue", h);
        object.put("saturation", s);
        object.put("brightness", b);
        return object;
    }
}
