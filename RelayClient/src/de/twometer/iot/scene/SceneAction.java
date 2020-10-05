package de.twometer.iot.scene;

import de.twometer.iot.bridge.IValue;

public class SceneAction {

    private String deviceId;

    private String propertyId;

    private IValue value;

    public SceneAction(String deviceId, String propertyId, IValue value) {
        this.deviceId = deviceId;
        this.propertyId = propertyId;
        this.value = value;
    }

    public String getDeviceId() {
        return deviceId;
    }

    public void setDeviceId(String deviceId) {
        this.deviceId = deviceId;
    }

    public String getPropertyId() {
        return propertyId;
    }

    public void setPropertyId(String propertyId) {
        this.propertyId = propertyId;
    }

    public IValue getValue() {
        return value;
    }

    public void setValue(IValue value) {
        this.value = value;
    }
}
