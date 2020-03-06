package de.twometer.iot.ui.bridge;

import androidx.annotation.StringRes;

class InfoItem {

    @StringRes
    private int headerRes;

    private String value;

    InfoItem(@StringRes int headerRes, String value) {
        this.headerRes = headerRes;
        this.value = value;
    }

    InfoItem(@StringRes int headerRes, int value) {
        this.headerRes = headerRes;
        this.value = String.valueOf(value);
    }

    @StringRes
    int getHeaderRes() {
        return headerRes;
    }

    String getValue() {
        return value;
    }

}
