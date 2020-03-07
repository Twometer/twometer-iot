package de.twometer.iot.handler;

import de.twometer.iot.model.ModeProperty;
import de.twometer.iot.model.Property;

public class PropertyMapper {

    public static String getPropertyName(Property property) {
        if (property instanceof ModeProperty)
            return "mode";

        switch (property.getName()) {
            case "Device.PowerState":
                return "powerState";
            case "Lamp.Brightness":
                return "brightness";
            case "Lamp.Color":
                return "color";
        }
        return null;
    }

    public static String getInterfaceName(Property property) {
        if (property instanceof ModeProperty)
            return "Alexa.ModeController";
        switch (property.getName()) {
            case "Device.PowerState":
                return "Alexa.PowerController";
            case "Lamp.Brightness":
                return "Alexa.BrightnessController";
            case "Lamp.Color":
                return "Alexa.ColorController";
        }
        return null;
    }

}
