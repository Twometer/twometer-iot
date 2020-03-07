package de.twometer.iot.json;

import org.json.JSONArray;
import org.json.JSONObject;

public class JSONStatic {

    public static JSONStaticObj newObject() {
        return new JSONStaticObj();
    }

    public static JSONStaticArray newArray() {
        return new JSONStaticArray();
    }

    public static class JSONStaticObj {

        private JSONObject object = new JSONObject();

        public JSONStaticObj put(String key, String value) {
            object.put(key, value);
            return this;
        }

        public JSONStaticObj put(String key, int value) {
            object.put(key, value);
            return this;
        }

        public JSONStaticObj put(String key, JSONObject value) {
            object.put(key, value);
            return this;
        }


        public JSONStaticObj put(String key, JSONArray value) {
            object.put(key, value);
            return this;
        }

        public JSONStaticObj put(String key, JSONStaticObj value) {
            object.put(key, value.object);
            return this;
        }

        public JSONStaticObj put(String key, JSONStaticArray value) {
            object.put(key, value.array);
            return this;
        }

        public String string() {
            return object.toString();
        }

        public JSONObject object() {
            return object;
        }

    }

    public static class JSONStaticArray {

        private JSONArray array = new JSONArray();

        public JSONStaticArray put(String key, JSONObject value) {
            array.put(value);
            return this;
        }

        public JSONStaticArray put(String value) {
            array.put(value);
            return this;
        }

        public JSONStaticArray put(JSONStaticArray array) {
            this.array.put(array);
            return this;
        }

        public JSONStaticArray put(JSONStaticObj obj) {
            this.array.put(obj);
            return this;
        }

    }

}
