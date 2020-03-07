package de.twometer.iot.handler;

import de.twometer.iot.alexa.IResponse;
import de.twometer.iot.net.BridgeClient;

public interface IHandler {

    IResponse handle(Request request, BridgeClient client);

    String getNamespace();

}
