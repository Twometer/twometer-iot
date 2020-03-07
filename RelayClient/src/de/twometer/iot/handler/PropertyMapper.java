package de.twometer.iot.handler;

import de.twometer.iot.model.ModeProperty;
import de.twometer.iot.model.Property;

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

    public static String getControllerName(Property property) {
        if (property instanceof ModeProperty)
            return "Alexa.ModeController";

        return findMapping(property).alexaController;
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
        private String alexaController;
        private String alexaProperty;

        public Mapping(String propertyName, String alexaController, String alexaProperty) {
            this.propertyName = propertyName;
            this.alexaController = alexaController;
            this.alexaProperty = alexaProperty;
        }
    }

}
