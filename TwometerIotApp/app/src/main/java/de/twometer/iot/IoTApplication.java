package de.twometer.iot;

import android.app.Application;

public class IoTApplication extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        IoTContext.getInstance().initialize(getApplicationContext());
    }
}
