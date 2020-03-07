package de.twometer.iot.bridge.values;

import de.twometer.iot.bridge.IValue;
import org.json.JSONObject;

public class JsonValue implements IValue {

    private JSONObject obj;

    public JsonValue(JSONObject obj) {
        this.obj = obj;
    }

    @Override
    public JSONObject toJson() {
        return obj;
    }

}
