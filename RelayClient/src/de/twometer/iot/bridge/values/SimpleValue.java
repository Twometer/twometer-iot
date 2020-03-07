package de.twometer.iot.bridge.values;

import de.twometer.iot.bridge.IValue;
import org.json.JSONObject;

public class SimpleValue<T> implements IValue {

    private String name;
    private T value;

    public SimpleValue(String name, T value) {
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
