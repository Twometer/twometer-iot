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
import de.twometer.iot.handler.base.IHandler;
import de.twometer.iot.handler.util.PropertyMapper;
import de.twometer.iot.net.BridgeClient;
import de.twometer.iot.scene.Scene;
import de.twometer.iot.scene.SceneManager;
import org.json.JSONArray;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import static de.twometer.iot.handler.util.PropertyMapper.getPropertyName;
import static de.twometer.iot.json.JSONStatic.newObject;

public class DiscoveryHandler implements IHandler {

    @Override
    public IResponse handle(Request request, BridgeClient client, SceneManager ext) {
        Device[] deviceList = client.getDevices();
        List<Endpoint> endpoints = new ArrayList<>();
        for (Device device : deviceList) {
            List<Property> properties = client.getProperties(device.getDeviceId());
            if (properties == null)
                continue;
            List<Capability> capabilities = new ArrayList<>();

            for (Property property : properties)
                capabilities.add(convert(property));

            Capability capability = new Capability();
            capability.setType("AlexaInterface");
            capability.set_interface("Alexa");
            capability.setVersion("3");
            capabilities.add(capability);

            endpoints.add(new Endpoint(device.getDeviceId(), device.getManufacturer(), device.getDescription(), device.getPreferredName(), new String[]{device.getType()}, capabilities.toArray(new Capability[]{})));
        }

        for (Scene scene : ext.getScenes()) {
            endpoints.add(buildSceneEndpoint(scene.getId(), scene.getFriendlyName(), scene.getDescription(), scene.isCanSwitchOff()));
        }

        try {
            return new GenericResponse(request.getCorrelationToken(), getNamespace(), "Discover.Response", newObject().put("endpoints", new JSONArray(new ObjectMapper().writeValueAsString(endpoints))).object());
        } catch (JsonProcessingException e) {
            e.printStackTrace();
            return new ErrorResponse(request, ErrorType.INTERNAL_ERROR, e.toString());
        }


    }

    private Endpoint buildSceneEndpoint(String endpointId, String friendlyName, String description, boolean canDeactivate) {
        Capability.SceneCap c = new Capability.SceneCap();
        c.setType("AlexaInterface");
        c.set_interface("Alexa.SceneController");
        c.setVersion("3");
        c.supportsDeactivation = canDeactivate;

        Capability[] caps = new Capability[]{c};
        Endpoint ep = new Endpoint(endpointId, "Twometer Applications", description, friendlyName, new String[]{"ACTIVITY_TRIGGER"}, caps);
        return ep;
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
