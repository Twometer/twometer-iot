package de.twometer.iot.handler;

import de.twometer.iot.alexa.model.ErrorType;
import de.twometer.iot.alexa.response.ErrorResponse;
import de.twometer.iot.alexa.response.IResponse;
import de.twometer.iot.alexa.response.StateResponse;
import de.twometer.iot.bridge.Property;
import de.twometer.iot.ext.ExtensionManager;
import de.twometer.iot.handler.base.IHandler;
import de.twometer.iot.handler.util.PropertyMapper;
import de.twometer.iot.handler.util.PropertyProcessor;
import de.twometer.iot.net.BridgeClient;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

public class AlexaHandler implements IHandler {

    @Override
    public IResponse handle(Request request, BridgeClient client, ExtensionManager ext) {
        if (request.getAction().equals("ReportState")) {
            List<Property> properties = client.getProperties(request.getEndpoint());
            if (properties == null) {
                return new ErrorResponse(request, ErrorType.ENDPOINT_UNREACHABLE, "Endpoint device is currently offline");
            }

            List<StateResponse.PropertyItem> values = new ArrayList<>();
            for (Property property : properties) {
                JSONObject value = client.getProperty(request.getEndpoint(), property.getName());
                String namespace = PropertyMapper.getPropertyNamespace(property);
                String propName = PropertyMapper.getPropertyName(property);
                values.add(new StateResponse.PropertyItem(namespace, propName, unwrap(value)));
            }
            return new StateResponse(request.getCorrelationToken(), "StateReport", values);
        }

        System.out.println("Cannot handle Alexa::" + request.getAction());
        return new ErrorResponse(request, ErrorType.INVALID_DIRECTIVE, "Cannot handle directive Alexa::" + request.getAction());
    }

    private Object unwrap(JSONObject object) {
        if (object.length() == 1)
            return PropertyProcessor.process(object.get(object.keys().next()));
        else
            return object;
    }

    @Override
    public String getNamespace() {
        return "Alexa";
    }

}
