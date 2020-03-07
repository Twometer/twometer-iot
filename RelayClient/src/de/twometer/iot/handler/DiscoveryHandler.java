package de.twometer.iot.handler;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import de.twometer.iot.alexa.AlexaResponse;
import de.twometer.iot.alexa.Capability;
import de.twometer.iot.alexa.Endpoint;
import de.twometer.iot.model.Device;
import de.twometer.iot.model.ModeProperty;
import de.twometer.iot.model.Property;
import de.twometer.iot.net.BridgeClient;
import org.json.JSONArray;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import static de.twometer.iot.json.JSONStatic.newObject;

public class DiscoveryHandler implements IHandler {

    @Override
    public AlexaResponse handle(String action, JSONObject payload, BridgeClient client) {
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
            return new AlexaResponse(getNamespace(), "Discover.Response", newObject().put("endpoints", new JSONArray(new ObjectMapper().writeValueAsString(endpoints))).object());
        } catch (JsonProcessingException e) {
            e.printStackTrace();
            return null;
        }


    }

    private Capability convert(Property property) {
        Capability capability = new Capability();
        capability.setType("AlexaInterface");
        capability.set_interface(getInterfaceName(property));
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

    private String getPropertyName(Property property) {
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

    private String getInterfaceName(Property property) {
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

    @Override
    public String getNamespace() {
        return "Alexa.Discovery";
    }

}
