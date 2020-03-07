package de.twometer.iot.handler.util;

import de.twometer.iot.bridge.ModeProperty;
import de.twometer.iot.bridge.Property;

import java.util.ArrayList;
import java.util.List;

public class PropertyMapper {

    private static List<Mapping> mappings = new ArrayList<>();

    static {
        map("Device.PowerState", "Alexa.PowerController", "powerState");
        map("Lamp.Brightness", "Alexa.BrightnessController", "brightness");
        map("Lamp.Color", "Alexa.ColorController", "color");
    }

    public static String getPropertyName(Property property) {
        if (property instanceof ModeProperty)
            return "mode";

        return findMapping(property).alexaProperty;
    }

    public static String getPropertyNamespace(Property property) {
        if (property instanceof ModeProperty)
            return "Alexa.ModeController";

        return findMapping(property).alexaNamespace;
    }

    private static Mapping findMapping(Property property) {
        for (Mapping mapping : mappings) {
            if (mapping.propertyName.equals(property.getName()))
                return mapping;
        }
        throw new IllegalArgumentException("Unknown property: " + property.getName());
    }

    private static void map(String propertyName, String controller, String mappedProp) {
        mappings.add(new Mapping(propertyName, controller, mappedProp));
    }

    private static class Mapping {
        private String propertyName;
        private String alexaNamespace;
        private String alexaProperty;

        public Mapping(String propertyName, String alexaNamespace, String alexaProperty) {
            this.propertyName = propertyName;
            this.alexaNamespace = alexaNamespace;
            this.alexaProperty = alexaProperty;
        }
    }

}
