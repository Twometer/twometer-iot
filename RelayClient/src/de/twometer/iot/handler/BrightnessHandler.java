package de.twometer.iot.handler;

import de.twometer.iot.handler.base.SimpleHandler;

public class BrightnessHandler extends SimpleHandler {

    public BrightnessHandler() {
        super("brightness", "Lamp.Brightness", ValueType.PRIMITIVE);
    }

    @Override
    public String getNamespace() {
        return "Alexa.BrightnessController";
    }

}
