package de.twometer.iot.model;

import org.jetbrains.annotations.NotNull;

public class Device {

    private String uuid;

    private String name;

    private DeviceType type;

    private String manufacturer;

    private String friendlyName;

    public Device(String uuid, String name, String type, String manufacturer, String friendlyName) {
        this.uuid = uuid;
        this.name = name;
        this.type = DeviceType.fromString(type);
        this.manufacturer = manufacturer;
        this.friendlyName = friendlyName;
    }

    public String getUuid() {
        return uuid;
    }

    public String getName() {
        return name;
    }

    public DeviceType getType() {
        return type;
    }

    public String getManufacturer() {
        return manufacturer;
    }

    public String getFriendlyName() {
        return friendlyName;
    }

    public String getPreferredName() {
        return friendlyName == null || friendlyName.isEmpty() ? name : friendlyName;
    }

    @NotNull
    @Override
    public String toString() {
        return "Device{" +
                "uuid='" + uuid + '\'' +
                ", name='" + name + '\'' +
                ", type='" + type + '\'' +
                ", manufacturer='" + manufacturer + '\'' +
                ", friendlyName='" + friendlyName + '\'' +
                '}';
    }
}
