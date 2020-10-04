package de.twometer.iot.handler.base;

import de.twometer.iot.alexa.response.IResponse;
import de.twometer.iot.ext.ExtensionManager;
import de.twometer.iot.handler.Request;
import de.twometer.iot.net.BridgeClient;

public interface IHandler {

    IResponse handle(Request request, BridgeClient client, ExtensionManager ext);

    String getNamespace();

}
