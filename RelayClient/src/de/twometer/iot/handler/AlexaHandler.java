package de.twometer.iot.handler;

import de.twometer.iot.alexa.ErrorResponse;
import de.twometer.iot.alexa.ErrorType;
import de.twometer.iot.alexa.IResponse;
import de.twometer.iot.alexa.StateUpdateResponse;
import de.twometer.iot.model.Property;
import de.twometer.iot.net.BridgeClient;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

public class AlexaHandler implements IHandler {

    @Override
    public IResponse handle(Request request, BridgeClient client) {
        if (request.getAction().equals("ReportState")) {
            List<Property> properties = client.getProperties(request.getEndpoint());
            List<StateUpdateResponse.PropertyItem> values = new ArrayList<>();
            for (Property property : properties) {
                JSONObject value = client.getProperty(request.getEndpoint(), property.getName());
                String namespace = PropertyMapper.getInterfaceName(property);
                values.add(new StateUpdateResponse.PropertyItem(namespace, property.getName(), unwrap(value)));
            }
            return new StateUpdateResponse("StateReport", values);
        }

        System.out.println("Cannot handle Alexa::" + request.getAction());
        return new ErrorResponse(request.getCorrelationToken(), request.getEndpoint(), ErrorType.INVALID_DIRECTIVE, "Cannot handle directive Alexa::" + request.getAction());
    }

    private Object unwrap(JSONObject object) {
        if (object.length() == 1)
            return object.get(object.keys().next());
        else
            return object;
    }

    @Override
    public String getNamespace() {
        return "Alexa";
    }

}
