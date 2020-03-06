package de.twometer.iot.model;

import de.twometer.iot.json.JSONName;

public class Device {

    @JSONName("deviceId")
    private String deviceId;

    @JSONName("modelName")
    private String modelName;

    @JSONName("manufacturer")
    private String manufacturer;

    @JSONName("description")
    private String description;

    @JSONName("type")
    private String type;

    @JSONName("online")
    private boolean online;

    public String getDeviceId() {
        return deviceId;
    }

    public void setDeviceId(String deviceId) {
        this.deviceId = deviceId;
    }

    public String getModelName() {
        return modelName;
    }

    public void setModelName(String modelName) {
        this.modelName = modelName;
    }

    public String getManufacturer() {
        return manufacturer;
    }

    public void setManufacturer(String manufacturer) {
        this.manufacturer = manufacturer;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public boolean isOnline() {
        return online;
    }

    public void setOnline(boolean online) {
        this.online = online;
    }

    @Override
    public String toString() {
        return "Device{" +
                "deviceId='" + deviceId + '\'' +
                ", modelName='" + modelName + '\'' +
                ", manufacturer='" + manufacturer + '\'' +
                ", description='" + description + '\'' +
                ", type='" + type + '\'' +
                ", online=" + online +
                '}';
    }
}
