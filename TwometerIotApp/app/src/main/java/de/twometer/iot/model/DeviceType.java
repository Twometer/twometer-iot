package de.twometer.iot.model;

import androidx.annotation.DrawableRes;
import androidx.annotation.StringRes;

import org.jetbrains.annotations.NotNull;

import de.twometer.iot.R;

public enum DeviceType {
    LIGHT_GENERIC("lights/generic", R.string.device_lights_generic, R.mipmap.ic_iot_lamp),
    LIGHT_STRIPE("lights/stripe", R.string.device_lights_stripe, R.mipmap.ic_iot_leds),
    SWITCH_GENERIC("switch/generic", R.string.device_switch_generic, R.mipmap.ic_iot_switch),
    SWITCH_MULTI("switch/multi", R.string.device_switch_multi, R.mipmap.ic_iot_switch_bar),
    BUTTON_GENERIC("button/generic", R.string.device_button_generic, R.mipmap.ic_iot_button),
    BUTTON_MULTI("button/multi", R.string.device_button_multi, R.mipmap.ic_iot_button_bar);

    private String value;

    @StringRes
    private int titleRes;

    @DrawableRes
    private int imageRes;

    DeviceType(String value, @StringRes int titleRes, @DrawableRes int imageRes) {
        this.value = value;
        this.titleRes = titleRes;
        this.imageRes = imageRes;
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

    public int getTitleRes() {
        return titleRes;
    }

    public int getImageRes() {
        return imageRes;
    }
}
