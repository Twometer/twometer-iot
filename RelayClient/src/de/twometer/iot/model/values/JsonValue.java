package de.twometer.iot.model.values;

import de.twometer.iot.model.IValue;
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
