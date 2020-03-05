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
import de.twometer.iot.net.api.ApiClient;
import de.twometer.iot.net.discovery.BridgeDiscovery;

import static org.junit.Assert.assertEquals;

@RunWith(AndroidJUnit4.class)
public class DeviceTest {

    @Test
    public void runTest() throws IOException {
        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();
        String bridgeIp = BridgeDiscovery.discover(appContext);
        Bridge bridge = new Bridge(bridgeIp);
        ApiClient apiClient = new ApiClient(bridge);
        apiClient.loadBridgeInfo();

        assertEquals("Twometer IoT Bridge", bridge.getName());
        assertEquals("0.1.0", bridge.getVersion());

        List<Device> devices = apiClient.getDevices();
        for (Device device : devices)
            System.out.println(device.toString());
    }

}
