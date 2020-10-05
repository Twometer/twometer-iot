package de.twometer.iot.handler.base;

import de.twometer.iot.alexa.response.IResponse;
import de.twometer.iot.handler.Request;
import de.twometer.iot.net.BridgeClient;
import de.twometer.iot.scene.SceneManager;

public interface IHandler {

    IResponse handle(Request request, BridgeClient client, SceneManager ext);

    String getNamespace();

}
