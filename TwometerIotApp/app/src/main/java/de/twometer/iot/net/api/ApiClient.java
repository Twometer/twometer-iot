package de.twometer.iot.net.api;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.List;

import de.twometer.iot.model.Bridge;
import de.twometer.iot.model.DebugInfo;
import de.twometer.iot.model.Device;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

public class ApiClient {

    private static final MediaType JSON = MediaType.get("application/json; charset=utf-8");

    private OkHttpClient client;

    private Bridge bridge;

    public ApiClient(Bridge bridge) {
        this.bridge = bridge;
        this.client = new OkHttpClient();
    }

    public void loadBridgeInfo() {
        try {
            JSONObject info = new JSONObject(doGet(getBridgeEndpoint("")));
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

    public boolean renameDevice(Device device, String newName) {
        return renameDevice(device.getUuid(), newName);
    }

    public boolean renameDevice(String deviceId, String newName) {
        try {
            return isOk(doPost(String.format("%s?id=%s&name=%s", getBridgeEndpoint("name"), urlEncode(deviceId), urlEncode(newName)), ""));
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

    public DebugInfo getDebugInfo() {
        try {
            JSONObject object = new JSONObject(doGet(getBridgeEndpoint("debug")));
            return new DebugInfo(
                    object.getString("name"),
                    object.getString("version"),
                    object.getInt("registry_token_count"),
                    object.getInt("connected_device_count"),
                    object.getInt("registered_device_count"),
                    object.getString("mode"),
                    object.getString("wifi_ssid"),
                    object.getInt("wifi_rssi"),
                    object.getInt("system_time"),
                    object.getInt("last_ping_stime"),
                    object.getBoolean("pair_shutdown_sched")
            );
        } catch (JSONException | IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    private boolean isOk(String response) {
        try {
            return new JSONObject(response).getString("status").equalsIgnoreCase("ok");
        } catch (JSONException e) {
            return false;
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

    private String doPost(String url, String json) throws IOException {
        RequestBody body = RequestBody.create(json, JSON);
        Request request = new Request.Builder()
                .url(url)
                .post(body)
                .build();
        try (Response response = client.newCall(request).execute()) {
            return response.body().string();
        }
    }

    private String urlEncode(String val) {
        try {
            return URLEncoder.encode(val, "UTF8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
            return null;
        }
    }

}
