package de.twometer.iot;

import org.java_websocket.client.WebSocketClient;
import org.java_websocket.handshake.ServerHandshake;
import org.json.JSONObject;

import java.net.URI;
import java.net.URISyntaxException;

public class RelayClient {

    private static final String UPSTREAM_TOKEN = "Rcddvfdnctvl8LU9a9LKOrilcfL859pSpnsRF11M";

    public static void main(String[] args) throws URISyntaxException, InterruptedException {
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
            } catch (URISyntaxException | InterruptedException e) {
                e.printStackTrace();
            }
        };
    }

    private static String handleMessage(String message) {
        System.out.println("Handling message:");
        System.out.println(" " + message);

        return message;
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

                JSONObject replyObject = new JSONObject();
                replyObject.put("uuid", messageId);
                replyObject.put("response", reply);
                send(replyObject.toString());
            }
        }

        @Override
        public void onClose(int i, String s, boolean b) {
            System.out.println("Upstream client shutdown");

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
