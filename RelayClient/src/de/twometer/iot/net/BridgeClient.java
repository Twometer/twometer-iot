package de.twometer.iot.net;

import de.twometer.iot.json.JSONDeserializer;
import de.twometer.iot.model.Device;
import de.twometer.iot.model.IValue;
import de.twometer.iot.model.ModeProperty;
import de.twometer.iot.model.Property;
import okhttp3.*;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

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
                JSONObject device = devices.getJSONObject(i);
                result.add(JSONDeserializer.deserialize(Device.class, device));
            }
            return result;
        } catch (IOException | JSONException e) {
            e.printStackTrace();
            return null;
        }
    }

    public List<Property> getProperties(String deviceId) {
        try {
            JSONArray properties = new JSONArray(doGet(bridgeUrl + "/capabilities?id=" + deviceId));
            List<Property> result = new ArrayList<>();
            for (int i = 0; i < properties.length(); i++) {
                JSONObject property = properties.getJSONObject(i);
                String name = property.getString("name");
                String type = property.getString("type");

                Property item;
                if (Objects.equals(type, "regular")) {
                    item = new Property(name, type);
                } else if (Objects.equals(type, "mode")) {
                    item = new ModeProperty(name, type, property.getString("friendlyName"));

                    for (String key : property.keySet()) {
                        if (!key.equals("name") && !key.equals("type") && !key.equals("friendlyName"))
                            ((ModeProperty) item).getModes().put(key, property.getString(key));
                    }
                } else {
                    throw new IllegalArgumentException("Unknown type " + type);
                }

                result.add(item);
            }
            return result;
        } catch (IOException | JSONException e) {
            e.printStackTrace();
            return null;
        }
    }

    public JSONObject getProperty(String deviceId, String property) {
        try {
            return new JSONObject(doGet(bridgeUrl + "/get?id=" + deviceId + "&prop=" + property));
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    public boolean setProperty(String deviceId, String property, IValue value) {
        try {
            String url = bridgeUrl + "/set?id=" + deviceId + "&prop=" + property;
            return isOk(doPost(url, value.toJson().toString()));
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

    private boolean isOk(String reply) {
        try {
            return new JSONObject(reply).getString("status").equals("ok");
        } catch (Exception e) {
            return false;
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
