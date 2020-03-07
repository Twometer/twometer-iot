package de.twometer.iot.handler;

import de.twometer.iot.handler.base.SimpleHandler;

public class ColorHandler extends SimpleHandler {
    public ColorHandler() {
        super("color", "Lamp.Color", ValueType.JSON);
    }

    @Override
    public String getNamespace() {
        return "Alexa.ColorController";
    }
}
