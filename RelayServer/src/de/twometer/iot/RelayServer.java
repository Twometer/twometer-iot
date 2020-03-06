package de.twometer.iot;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;

import java.io.IOException;
import java.net.InetSocketAddress;

public class RelayServer {

    private static final int UPSTREAM_PORT = 38667;
    private static final int DOWNSTREAM_PORT = 38668;

    public static void main(String[] args) throws IOException {
        HttpServer server = HttpServer.create(new InetSocketAddress(UPSTREAM_PORT), 0);
        server.createContext("/", new RelayHandler());
        server.setExecutor(null);
        server.start();
    }

    private static class RelayHandler implements HttpHandler {

        @Override
        public void handle(HttpExchange httpExchange) throws IOException {

        }
    }


}
