package de.twometer.iot;


import android.content.Context;

import androidx.test.ext.junit.runners.AndroidJUnit4;
import androidx.test.platform.app.InstrumentationRegistry;

import org.junit.Test;
import org.junit.runner.RunWith;

import java.io.IOException;
import java.util.List;

import de.twometer.iot.model.Bridge;
import de.twometer.iot.model.Device;
import de.twometer.iot.model.Property;
import de.twometer.iot.model.values.RgbValue;
import de.twometer.iot.net.api.ApiClient;
import de.twometer.iot.net.discovery.BridgeDiscovery;

import static junit.framework.TestCase.assertTrue;
import static org.junit.Assert.assertFalse;

@RunWith(AndroidJUnit4.class)
public class PropertyTest {

    @Test
    public void test() throws IOException {
        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();
        String bridgeIp = BridgeDiscovery.discover(appContext);
        Bridge bridge = new Bridge(bridgeIp);
        ApiClient apiClient = new ApiClient(bridge);
        Device device = apiClient.getDevices().get(0);

        List<Property> properties = apiClient.getProperties(device.getUuid());
        for (Property prop : properties)
            System.err.println(prop.toString());

        assertTrue(apiClient.setProperty(device.getUuid(), "color", new RgbValue(255, 255, 42)));
        assertFalse(apiClient.setProperty(device.getUuid(), "color", new RgbValue(255, 255, -455)));
    }

}
