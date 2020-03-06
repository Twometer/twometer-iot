package de.twometer.iot;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;
import org.java_websocket.WebSocket;
import org.java_websocket.handshake.ClientHandshake;
import org.java_websocket.server.WebSocketServer;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;

public class RelayServer {

    private static final String AUTH_SCHEME = "X-TokenAuth";
    private static final String UPSTREAM_TOKEN = "U4UE8ytYa4CMAWIwNZQ8Hu3CGhiYGuh7XrMVv42J";
    private static final String DOWNSTREAM_TOKEN = "Rcddvfdnctvl8LU9a9LKOrilcfL859pSpnsRF11M";

    private static final int UPSTREAM_PORT = 38667;
    private static final int DOWNSTREAM_PORT = 38668;

    private Map<String, RelayMessage> relayMessages = new ConcurrentHashMap<>();

    private DownstreamServer downstreamServer;

    public static void main(String[] args) throws IOException {
        new RelayServer().start();
    }

    private void start() throws IOException {
        HttpServer upstreamServer = HttpServer.create(new InetSocketAddress(UPSTREAM_PORT), 0);
        upstreamServer.createContext("/", new RelayHandler());
        upstreamServer.setExecutor(null);
        upstreamServer.start();

        downstreamServer = new DownstreamServer(new InetSocketAddress(DOWNSTREAM_PORT));
        downstreamServer.run();
    }

    private class RelayHandler implements HttpHandler {

        @Override
        public void handle(HttpExchange httpExchange) throws IOException {
            String method = httpExchange.getRequestMethod();
            String header = httpExchange.getRequestHeaders().getFirst("Authorization");
            if (!method.equals("POST")) {
                httpExchange.close();
                return;
            }

            if (header == null || header.isEmpty()) {
                httpExchange.sendResponseHeaders(401, 0);
                return;
            }

            if (!header.equals(AUTH_SCHEME + " " + UPSTREAM_TOKEN)) {
                httpExchange.sendResponseHeaders(403, 0);
                return;
            }

            UUID messageId = UUID.randomUUID();
            String messageContent = IOUtils.readToEnd(httpExchange.getRequestBody());
            String response = downstreamServer.relayMessage(messageId, messageContent).await();

            if (response == null) {
                httpExchange.sendResponseHeaders(502, 0);
                return;
            }

            OutputStream os = httpExchange.getResponseBody();
            os.write(response.getBytes(StandardCharsets.UTF_8));
            os.close();
        }

    }

    private class DownstreamServer extends WebSocketServer {

        private List<InetSocketAddress> authenticatedAddresses;

        public DownstreamServer(InetSocketAddress address) {
            super(address);
            authenticatedAddresses = new ArrayList<>();
        }

        public Promise<String> relayMessage(UUID messageId, String message) {
            Promise<String> promise = new Promise<>();
            RelayMessage relayMessage = new RelayMessage(message, promise);
            relayMessages.put(messageId.toString(), relayMessage);

            JSONObject object = new JSONObject();
            object.put("uuid", messageId);
            object.put("message", message);

            for (WebSocket socket : getConnections())
                socket.send(object.toString());

            return promise;
        }

        @Override
        public void onOpen(WebSocket webSocket, ClientHandshake clientHandshake) {

        }

        @Override
        public void onClose(WebSocket webSocket, int i, String s, boolean b) {

        }

        @Override
        public void onMessage(WebSocket webSocket, String s) {
            if (s.equals(DOWNSTREAM_TOKEN)) {
                authenticatedAddresses.add(webSocket.getRemoteSocketAddress());
                return;
            }
            if (!authenticatedAddresses.contains(webSocket.getRemoteSocketAddress())) {
                webSocket.send("forbidden");
                return;
            }

            try {
                JSONObject object = new JSONObject(s);
                String messageId = object.getString("uuid");
                String response = object.getString("response");
                RelayMessage message = relayMessages.remove(messageId);
                if (message != null)
                    message.getResponsePromise().resolve(response);
                else {
                    webSocket.send("unknown message");
                    webSocket.close();
                }
            } catch (JSONException e) {
                webSocket.send("invalid frame");
                webSocket.close();
            }
        }

        @Override
        public void onError(WebSocket webSocket, Exception e) {

        }

        @Override
        public void onStart() {

        }
    }


}
