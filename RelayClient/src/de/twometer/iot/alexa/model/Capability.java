package de.twometer.iot.alexa.model;

import com.fasterxml.jackson.annotation.JsonProperty;

import java.util.ArrayList;
import java.util.List;

public class Capability {

    private String type;

    @JsonProperty("interface")
    private String _interface;

    private String instance;

    private String version;

    private Properties properties;

    private Resources capabilityResources;

    private Configuration configuration;

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String get_interface() {
        return _interface;
    }

    public void set_interface(String _interface) {
        this._interface = _interface;
    }

    public String getInstance() {
        return instance;
    }

    public void setInstance(String instance) {
        this.instance = instance;
    }

    public String getVersion() {
        return version;
    }

    public void setVersion(String version) {
        this.version = version;
    }

    public Properties getProperties() {
        return properties;
    }

    public void setProperties(Properties properties) {
        this.properties = properties;
    }

    public Resources getCapabilityResources() {
        return capabilityResources;
    }

    public void setCapabilityResources(Resources capabilityResources) {
        this.capabilityResources = capabilityResources;
    }

    public Configuration getConfiguration() {
        return configuration;
    }

    public void setConfiguration(Configuration configuration) {
        this.configuration = configuration;
    }

    public static class Configuration {
        public boolean ordered = false;

        public List<SupportedMode> supportedModes = new ArrayList<>();

        public static class SupportedMode {

            public String value;

            public Resources modeResources;

        }

    }

    public static class Resources {

        public List<FriendlyName> friendlyNames = new ArrayList<>();

    }

    public static class FriendlyName {
        @JsonProperty("@type")
        public String type;

        public Value value;

        public FriendlyName(String type, Value value) {
            this.type = type;
            this.value = value;
        }

        public static class Value {
            public String text;
            public String locale;

            public Value(String text, String locale) {
                this.text = text;
                this.locale = locale;
            }
        }
    }

    public static class Properties {
        public SupportedProperty[] supported;

        public boolean proactivelyReported = false;
        public boolean retrievable = true;

        public Properties(SupportedProperty[] supported) {
            this.supported = supported;
        }

        public static class SupportedProperty {
            public String name;

            public SupportedProperty(String name) {
                this.name = name;
            }
        }
    }
}
