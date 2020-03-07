package de.twometer.iot.handler.util;

public class PropertyProcessor {

    public static Object process(Object object) {
        if (object instanceof Boolean)
            return object.equals(Boolean.TRUE) ? "ON" : "OFF";
        return object;
    }

}
