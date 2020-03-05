package de.twometer.iot.net.api;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import de.twometer.iot.model.Bridge;
import de.twometer.iot.model.Device;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class ApiClient {

    private OkHttpClient client;

    private Bridge bridge;

    public ApiClient(Bridge bridge) {
        this.bridge = bridge;
        this.client = new OkHttpClient();
    }

    public void loadBridgeInfo() {
        try {
            String payload = doGet(getBridgeEndpoint(""));
            System.out.println(payload);
            JSONObject info = new JSONObject(payload);
            bridge.setName(info.getString("name"));
            bridge.setVersion(info.getString("version"));
        } catch (IOException | JSONException e) {
            e.printStackTrace();
        }
    }

    public List<Device> getDevices() {
        try {
            JSONArray devices = new JSONArray(doGet(getBridgeEndpoint("devices")));
            List<Device> result = new ArrayList<>();
            for (int i = 0; i < devices.length(); i++) {
                JSONObject device = (JSONObject) devices.get(i);

                result.add(new Device(
                        device.getString("uuid"),
                        device.getString("name"),
                        device.getString("type"),
                        device.getString("manufacturer"),
                        device.optString("friendly_name")
                ));
            }
            return result;
        } catch (IOException | JSONException e) {
            e.printStackTrace();
            return null;
        }
    }

    private String getBridgeEndpoint(String ep) {
        return String.format("http://%s/%s", bridge.getIp(), ep);
    }

    private String doGet(String url) throws IOException, JSONException {
        Request request = new Request.Builder()
                .url(url)
                .build();

        try (Response response = client.newCall(request).execute()) {
            return response.body().string();
        }
    }

}
