package de.twometer.iot;

import de.twometer.iot.handler.*;
import de.twometer.iot.handler.base.IHandler;
import de.twometer.iot.net.BridgeClient;
import de.twometer.iot.net.BridgeDiscovery;
import de.twometer.iot.rest.RestServer;
import de.twometer.iot.scene.Scene;
import de.twometer.iot.scene.SceneManager;
import org.java_websocket.client.WebSocketClient;
import org.java_websocket.handshake.ServerHandshake;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.Objects;

import static de.twometer.iot.security.Tokens.UPSTREAM_TOKEN;

public class RelayClient {

    private static String bridgeUrl;

    private static BridgeClient client;

    private static final IHandler[] handlers = new IHandler[]{
            new DiscoveryHandler(),
            new ColorHandler(),
            new BrightnessHandler(),
            new ModeHandler(),
            new AlexaHandler(),
            new PowerHandler(),
            new SceneHandler()
    };
    private static SceneManager sceneManager;
    private static RestServer restServer;

    public static void main(String[] args) throws URISyntaxException, IOException {
        if (bridgeUrl == null) {
            System.out.println("Discovering IoT bridge...");
            bridgeUrl = "http://" + BridgeDiscovery.discover();
            client = new BridgeClient(bridgeUrl);
            System.out.println("Discovered: " + bridgeUrl);
        }

        UpstreamClient upstreamClient = new UpstreamClient(new URI("wss://iot.twometer.de/websocket"));
        upstreamClient.connect();

        upstreamClient.disconnectListener = () -> {
            System.out.println("Connection lost, reconnecting in 10s");
            try {
                Thread.sleep(10000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            try {
                main(args);
            } catch (URISyntaxException | IOException e) {
                e.printStackTrace();
            }
        };

        if (restServer == null) {
            sceneManager = new SceneManager();
            restServer = new RestServer(8090);
            restServer.host("GET", "/devices", request -> client.getDevices());
            restServer.host("POST", "/scene", request -> {
                Scene requestScene = request.getBody(Scene.class);
                if (requestScene == null)
                    return "bad request";
                sceneManager.addScene(requestScene);
                sceneManager.save();
                return "ok";
            });
            restServer.host("DELETE", "/scene", request -> {
                SceneDeleteRequest r = request.getBody(SceneDeleteRequest.class);
                sceneManager.deleteScene(r.id);
                sceneManager.save();
                return "ok";
            });
            restServer.start();
            System.out.println("REST API online");
        }


    }

    private static String handleMessage(String namespace, Request request) {
        for (IHandler handler : handlers) {
            if (Objects.equals(handler.getNamespace(), namespace)) {
                return handler.handle(request, client, sceneManager).toJson().toString();
            }
        }
        System.out.println(" Couldn't figure out how to handle " + namespace + "::" + request.getAction());
        return "{\"error\": \"unknown_action\"}";
    }

    private static String handleMessage(String message) {
        System.out.println("Handling message:");
        System.out.println(" " + message);

        try {
            JSONObject object = new JSONObject(message).getJSONObject("directive");
            JSONObject header = object.getJSONObject("header");

            String namespace = header.getString("namespace");
            String name = header.getString("name");
            String payloadVersion = header.getString("payloadVersion");
            String corToken = header.optString("correlationToken");
            String instance = header.optString("instance");

            if (!payloadVersion.equals("3")) {
                System.err.println(" ERR: Invalid payload version " + payloadVersion);
                return "";
            }

            String endpoint = object.has("endpoint") ? object.getJSONObject("endpoint").getString("endpointId") : "";
            Request request = new Request(name, endpoint, instance, corToken, object.getJSONObject("payload"));

            return handleMessage(namespace, request);

        } catch (JSONException e) {
            System.out.println(" ERR: Message was malformed");
            e.printStackTrace();
            return "{\"error\": \"malformed_request\"}";
        }
    }

    private static class SceneDeleteRequest {
        public String id;
    }

    private static class UpstreamClient extends WebSocketClient {

        public Runnable disconnectListener;

        public UpstreamClient(URI serverUri) {
            super(serverUri);
        }

        @Override
        public void onOpen(ServerHandshake serverHandshake) {
            System.out.println("Upstream client online");
            send(UPSTREAM_TOKEN);
        }

        @Override
        public void onMessage(String s) {
            JSONObject object = new JSONObject(s);
            if (object.has("status")) {
                // It's a status message
                System.out.println("Status changed: " + object.get("status"));
            } else {
                // It's a business message
                String messageId = object.getString("uuid");
                String payload = object.getString("message");
                String reply = handleMessage(payload);

                System.out.println(" Sending reply: " + reply);

                JSONObject replyObject = new JSONObject();
                replyObject.put("uuid", messageId);
                replyObject.put("response", reply);
                send(replyObject.toString());
            }
        }

        @Override
        public void onClose(int i, String s, boolean b) {
            System.out.println("Upstream client shutdown: " + s + ", " + i + ";  " + b);

            handleDisconnect();
        }

        @Override
        public void onError(Exception e) {
            System.out.println("Upstream client error");
            e.printStackTrace();
            handleDisconnect();
        }

        private void handleDisconnect() {
            if (disconnectListener == null) return;
            disconnectListener.run();
            disconnectListener = null;
        }

    }

}
