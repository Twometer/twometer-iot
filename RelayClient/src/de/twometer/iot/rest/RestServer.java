package de.twometer.iot.rest;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.sun.net.httpserver.HttpServer;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

public class RestServer {

    private final int port;

    private final HttpServer server;
    private final List<HostEntry> entries = new ArrayList<>();

    public RestServer(int port) throws IOException {
        this.port = port;
        server = HttpServer.create(new InetSocketAddress(port), 0);
        server.setExecutor(null);
    }

    public void host(String method, String route, RestHandler handler) {
        server.createContext(route, httpExchange -> {
            if (!httpExchange.getRequestMethod().equals(method))
                return;

            ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
            byte[] buf = new byte[1024];
            int read;
            while ((read = httpExchange.getRequestBody().read(buf)) > 0)
                outputStream.write(buf, 0, read);

            String req = new String(outputStream.toByteArray(), StandardCharsets.UTF_8);
            String response;
            ObjectMapper mapper = new ObjectMapper();
            try {
                Object reply = handler.handle(new HttpRequest(req, httpExchange.getRequestURI().getQuery()));
                response = mapper.writeValueAsString(reply);
            } catch (Throwable t) {

                response = mapper.writeValueAsString(new ErrorReply(t.toString()));
            }
            httpExchange.sendResponseHeaders(200, response.getBytes().length);
            OutputStream r = httpExchange.getResponseBody();
            r.write(response.getBytes());
            r.close();
        });
    }

    public void start() {
        server.start();
        /*HttpConfig config = new HttpConfig();
        config.setPort(port);
        server = new HttpServer(config);
        server.setCallback(request -> {
            for (HostEntry entry : entries)
                if (entry.method.equals(request.getMethod()) && entry.route.equals(request.getRequestUri()))
                {
                    return new JsonResponse(entry.handler.handle(request));
                }
            System.out.println("Unknown request");
            return new ErrorResponse(StatusCode.NotFound, "404 not found");
        });
        try {
            server.start();
        } catch (IOException e) {
            e.printStackTrace();
        }*/
    }

    public void stop() {

    }

    private static class HostEntry {
        String method;
        String route;
        RestHandler handler;

        public HostEntry(String method, String route, RestHandler handler) {
            this.method = method;
            this.route = route;
            this.handler = handler;
        }
    }

    private static class ErrorReply {
        public String message;

        public ErrorReply(String message) {
            this.message = message;
        }
    }

}
