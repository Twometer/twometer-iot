package de.twometer.iot;

import android.content.Context;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Log;

import java.io.IOException;

import de.twometer.iot.model.Bridge;
import de.twometer.iot.net.api.ApiClient;
import de.twometer.iot.net.discovery.BridgeDiscovery;

public class IoTContext {

    private static final String TAG = "IoTContext";

    private static final IoTContext instance = new IoTContext();

    private Handler backgroundHandler;

    private Bridge bridge;

    private ApiClient client;

    public static IoTContext getInstance() {
        return instance;
    }

    void initialize(Context context) {
        HandlerThread thread = new HandlerThread("BackgroundThread");
        thread.setDaemon(true);
        thread.start();
        backgroundHandler = new Handler(thread.getLooper());

        backgroundHandler.post(() -> {
            try {
                bridge = new Bridge(BridgeDiscovery.discover(context));
                client = new ApiClient(bridge);
                client.loadBridgeInfo();
                Log.i(TAG, "Connected to IoT bridge");
            } catch (IOException e) {
                e.printStackTrace();
            }
        });
    }

    public Handler getBackgroundHandler() {
        return backgroundHandler;
    }

    public Bridge getBridge() {
        return bridge;
    }

    public ApiClient getClient() {
        return client;
    }

}
