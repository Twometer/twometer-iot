package de.twometer.iot.bridge;

import java.util.HashMap;
import java.util.Map;

public class ModeProperty extends Property {

    private String friendlyName;

    private Map<String, String> modes;

    public ModeProperty(String name, String type, String friendlyName) {
        super(name, type);
        this.friendlyName = friendlyName;
        this.modes = new HashMap<>();
    }

    public String getFriendlyName() {
        return friendlyName;
    }

    public void setFriendlyName(String friendlyName) {
        this.friendlyName = friendlyName;
    }

    public Map<String, String> getModes() {
        return modes;
    }

    @Override
    public String toString() {
        return "ModeProperty{" +
                "friendlyName='" + friendlyName + '\'' +
                ", modes=" + modes +
                '}';
    }
}
