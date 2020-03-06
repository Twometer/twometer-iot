package de.twometer.iot.json;

import org.json.JSONObject;

import java.lang.reflect.Field;

public class JSONDeserializer {

    public static <T> T deserialize(Class<T> clazz, String json) {
        JSONObject object = new JSONObject(json);
        try {
            T obj = clazz.newInstance();

            for (Field field : clazz.getDeclaredFields()) {
                boolean acc = field.isAccessible();
                field.setAccessible(true);

                JSONName annotation = field.getAnnotation(JSONName.class);
                if (annotation == null)
                    continue;

                field.set(obj, object.get(annotation.value()));

                field.setAccessible(acc);
            }

            return obj;
        } catch (InstantiationException | IllegalAccessException e) {
            e.printStackTrace();
            return null;
        }
    }

}
