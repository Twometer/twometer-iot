package de.twometer.iot.handler;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import de.twometer.iot.alexa.model.Capability;
import de.twometer.iot.alexa.model.Endpoint;
import de.twometer.iot.alexa.model.ErrorType;
import de.twometer.iot.alexa.response.ErrorResponse;
import de.twometer.iot.alexa.response.GenericResponse;
import de.twometer.iot.alexa.response.IResponse;
import de.twometer.iot.bridge.Device;
import de.twometer.iot.bridge.ModeProperty;
import de.twometer.iot.bridge.Property;
import de.twometer.iot.net.BridgeClient;
import org.json.JSONArray;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import static de.twometer.iot.handler.PropertyMapper.getPropertyName;
import static de.twometer.iot.json.JSONStatic.newObject;

public class DiscoveryHandler implements IHandler {

    @Override
    public IResponse handle(Request request, BridgeClient client) {
        Device[] deviceList = client.getDevices();
        List<Endpoint> endpoints = new ArrayList<>();
        for (Device device : deviceList) {
            List<Property> properties = client.getProperties(device.getDeviceId());
            List<Capability> capabilities = new ArrayList<>();

            for (Property property : properties)
                capabilities.add(convert(property));

            endpoints.add(new Endpoint(device.getDeviceId(), device.getManufacturer(), device.getDescription(), device.getPreferredName(), new String[]{device.getType()}, capabilities.toArray(new Capability[]{})));
        }

        try {
            return new GenericResponse(request.getCorrelationToken(), getNamespace(), "Discover.Response", newObject().put("endpoints", new JSONArray(new ObjectMapper().writeValueAsString(endpoints))).object());
        } catch (JsonProcessingException e) {
            e.printStackTrace();
            return new ErrorResponse(request.getCorrelationToken(), request.getEndpoint(), ErrorType.INTERNAL_ERROR, e.toString());
        }


    }

    private Capability convert(Property property) {
        Capability capability = new Capability();
        capability.setType("AlexaInterface");
        capability.set_interface(PropertyMapper.getPropertyNamespace(property));
        capability.setVersion("3");
        capability.setInstance(property.getName());
        capability.setProperties(new Capability.Properties(new Capability.Properties.SupportedProperty[]{new Capability.Properties.SupportedProperty(getPropertyName(property))}));

        if (property instanceof ModeProperty) {
            String fn = ((ModeProperty) property).getFriendlyName();
            Map<String, String> config = ((ModeProperty) property).getModes();

            capability.setCapabilityResources(new Capability.Resources());
            capability.getCapabilityResources().friendlyNames.add(new Capability.FriendlyName("text", new Capability.FriendlyName.Value(fn, "de-DE")));
            capability.setConfiguration(new Capability.Configuration());

            for (Map.Entry<String, String> entry : config.entrySet()) {
                Capability.Configuration.SupportedMode mode = new Capability.Configuration.SupportedMode();
                mode.value = entry.getKey();
                mode.modeResources = new Capability.Resources();
                mode.modeResources.friendlyNames.add(new Capability.FriendlyName("text", new Capability.FriendlyName.Value(entry.getValue(), "de-DE")));
                capability.getConfiguration().supportedModes.add(mode);
            }
        }

        return capability;
    }



    @Override
    public String getNamespace() {
        return "Alexa.Discovery";
    }

}
