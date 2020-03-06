struct HttpResponse {
    bool ok;
    int code;
    String data;
};

HttpResponse http_post(String endpoint, String payload) {
    WiFiClient wifi;
    HTTPClient http;
    if (http.begin(wifi, endpoint)) {
        http.addHeader("Content-Type", "application/json");
        int statusCode = http.POST(payload);
        if (statusCode > 0) {
            String reply = http.getString();
            return { true, statusCode, reply };
        } else {
            return { false, statusCode };
        }
        http.end();
    }
    return { false };
}

HttpResponse http_put(String endpoint, String payload) {
    WiFiClient wifi;
    HTTPClient http;
    if (http.begin(wifi, endpoint)) {
        http.addHeader("Content-Type", "application/json");
        int statusCode = http.PUT(payload);
        if (statusCode > 0) {
            String reply = http.getString();
            return { true, statusCode, reply };
        } else {
            return { false, statusCode };
        }
        http.end();
    }
    return { false };
}

HttpResponse http_get(String endpoint) {
    WiFiClient wifi;
    HTTPClient http;
    if (http.begin(wifi, endpoint)) {
        int statusCode = http.GET();
        if (statusCode > 0) {
            String reply = http.getString();
            return { true, statusCode, reply };
        } else {
            return { false, statusCode };
        }
        http.end();
    }
    return { false };
}