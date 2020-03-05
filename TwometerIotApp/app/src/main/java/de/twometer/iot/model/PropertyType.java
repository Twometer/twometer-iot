package de.twometer.iot.model;

import org.jetbrains.annotations.NotNull;

public enum PropertyType {
    INT("int"),
    BOOL("bool"),
    FLOAT("float"),
    STRING("string"),
    COLOR_RGB("color/rgb"),
    COLOR_RGBW("color/rgbw"),
    COLOR_RGB2W("color/rgb2w"),
    CUSTOM("custom");

    private String value;

    PropertyType(String value) {
        this.value = value;
    }

    public static PropertyType fromString(String str) {
        for (PropertyType type : values())
            if (type.value.equals(str))
                return type;
        return null;
    }

    @NotNull
    @Override
    public String toString() {
        return value;
    }

}
