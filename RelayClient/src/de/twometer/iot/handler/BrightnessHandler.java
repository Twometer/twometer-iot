package de.twometer.iot.handler;

import de.twometer.iot.alexa.response.IResponse;
import de.twometer.iot.bridge.values.SimpleValue;
import de.twometer.iot.handler.base.SimpleHandler;
import de.twometer.iot.net.BridgeClient;
import de.twometer.iot.scene.SceneManager;

public class BrightnessHandler extends SimpleHandler {

    public BrightnessHandler() {
        super("brightness", "Lamp.Brightness", ValueType.PRIMITIVE);
    }

    @Override
    public String getNamespace() {
        return "Alexa.BrightnessController";
    }

    @Override
    public IResponse handle(Request request, BridgeClient client, SceneManager ext) {
        if (request.getPayload().getInt("brightness") != 0) {
            client.setProperty(request.getEndpoint(), "Device.PowerState", new SimpleValue<>("powerState", true));
        }
        return super.handle(request, client, ext);
    }
}
