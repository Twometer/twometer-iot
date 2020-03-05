package de.twometer.iot.model;

public class Property {

    private String name;
    private PropertyType type;

    public Property(String name, String type) {
        this.name = name;
        this.type = PropertyType.fromString(type);
    }

    public String getName() {
        return name;
    }

    public PropertyType getType() {
        return type;
    }

}
