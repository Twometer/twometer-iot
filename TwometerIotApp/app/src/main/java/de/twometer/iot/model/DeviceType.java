package de.twometer.iot.model;

import org.jetbrains.annotations.NotNull;

public enum DeviceType {
    LIGHT_GENERIC("lights/generic"),
    LIGHT_STRIPE("lights/stripe"),
    SWITCH_GENERIC("switch/generic"),
    SWITCH_MULTI("switch/multi"),
    BUTTON_GENERIC("button/generic"),
    BUTTON_MULTI("button/multi");

    private String value;

    DeviceType(String value) {
        this.value = value;
    }

    public static DeviceType fromString(String str) {
        for (DeviceType type : values())
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
