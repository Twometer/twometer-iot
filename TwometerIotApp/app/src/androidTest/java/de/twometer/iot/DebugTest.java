package de.twometer.iot;

import android.content.Context;

import androidx.test.ext.junit.runners.AndroidJUnit4;
import androidx.test.platform.app.InstrumentationRegistry;

import org.junit.Test;
import org.junit.runner.RunWith;

import java.io.IOException;

import de.twometer.iot.model.Bridge;
import de.twometer.iot.net.api.ApiClient;
import de.twometer.iot.net.discovery.BridgeDiscovery;

@RunWith(AndroidJUnit4.class)
public class DebugTest {

    @Test
    public void test() throws IOException {
        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();
        String bridgeIp = BridgeDiscovery.discover(appContext);
        Bridge bridge = new Bridge(bridgeIp);
        ApiClient apiClient = new ApiClient(bridge);

        System.err.println(apiClient.getDebugInfo().toString());
    }

}
