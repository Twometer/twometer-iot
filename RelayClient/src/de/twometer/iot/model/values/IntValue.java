package de.twometer.iot.model.values;

import de.twometer.iot.model.IValue;
import org.json.JSONObject;

public class IntValue implements IValue {

    private String name;
    private int value;

    public IntValue(String name, int value) {
        this.name = name;
        this.value = value;
    }

    @Override
    public JSONObject toJson() {
        JSONObject object = new JSONObject();
        object.put(name, value);
        return object;
    }
}
