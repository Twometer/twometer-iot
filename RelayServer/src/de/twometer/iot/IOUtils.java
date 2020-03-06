package de.twometer.iot;

import com.sun.net.httpserver.HttpExchange;

import java.io.*;
import java.nio.charset.StandardCharsets;

public class IOUtils {

    public static String readToEnd(InputStream stream) throws IOException {
        StringBuilder builder = new StringBuilder();
        BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
        String line;
        while ((line = reader.readLine()) != null)
            builder.append(line).append("\n");
        return builder.toString();
    }

    public static String wrapJsonResponse(String data) {
        return "{\"status\": \"" + data + "\"}";
    }

    public static void send(HttpExchange e, int code, String data) throws IOException {
        byte[] payload = data.getBytes(StandardCharsets.UTF_8);
        e.sendResponseHeaders(code, payload.length);
        OutputStream stream = e.getResponseBody();
        stream.write(payload);
        stream.close();
    }

}
