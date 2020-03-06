package de.twometer.iot.net;

import de.twometer.iot.json.JSONDeserializer;
import de.twometer.iot.model.Device;
import okhttp3.*;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.List;

public class BridgeClient {

    private static final MediaType JSON = MediaType.get("application/json; charset=utf-8");

    private OkHttpClient client;

    private String bridgeUrl;

    public BridgeClient(String bridgeUrl) {
        this.bridgeUrl = bridgeUrl;
        this.client = new OkHttpClient();
    }

    public List<Device> getDevices() {
        try {
            JSONArray devices = new JSONArray(doGet(bridgeUrl + "/devices"));
            List<Device> result = new ArrayList<>();
            for (int i = 0; i < devices.length(); i++) {
                JSONObject device = (JSONObject) devices.get(i);
                result.add(JSONDeserializer.deserialize(Device.class, device));
            }
            return result;
        } catch (IOException | JSONException e) {
            e.printStackTrace();
            return null;
        }
    }

    private String doGet(String url) throws IOException {
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
