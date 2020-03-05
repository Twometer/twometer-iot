package de.twometer.iot;

import android.content.Context;

import androidx.test.ext.junit.runners.AndroidJUnit4;
import androidx.test.platform.app.InstrumentationRegistry;

import org.junit.Test;
import org.junit.runner.RunWith;

import java.io.IOException;

import de.twometer.iot.net.discovery.BridgeDiscovery;

import static org.junit.Assert.assertEquals;

@RunWith(AndroidJUnit4.class)
public class DiscoveryTest {
    @Test
    public void discoverBridge() throws IOException {
        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();
        String bridgeIp = BridgeDiscovery.discover(appContext);

        assertEquals("192.168.2.115", bridgeIp);
    }
}