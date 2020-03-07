package de.twometer.iot.alexa.model;

public class Endpoint {

    private String endpointId;
    private String manufacturerName;
    private String description;
    private String friendlyName;
    private String[] displayCategories;
    private Capability[] capabilities;

    public Endpoint(String endpointId, String manufacturerName, String description, String friendlyName, String[] displayCategories, Capability[] capabilities) {
        this.endpointId = endpointId;
        this.manufacturerName = manufacturerName;
        this.description = description;
        this.friendlyName = friendlyName;
        this.displayCategories = displayCategories;
        this.capabilities = capabilities;
    }


    public String getEndpointId() {
        return endpointId;
    }

    public void setEndpointId(String endpointId) {
        this.endpointId = endpointId;
    }

    public String getManufacturerName() {
        return manufacturerName;
    }

    public void setManufacturerName(String manufacturerName) {
        this.manufacturerName = manufacturerName;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getFriendlyName() {
        return friendlyName;
    }

    public void setFriendlyName(String friendlyName) {
        this.friendlyName = friendlyName;
    }

    public String[] getDisplayCategories() {
        return displayCategories;
    }

    public void setDisplayCategories(String[] displayCategories) {
        this.displayCategories = displayCategories;
    }

    public Capability[] getCapabilities() {
        return capabilities;
    }

    public void setCapabilities(Capability[] capabilities) {
        this.capabilities = capabilities;
    }
}
