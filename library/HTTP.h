#ifndef _HTTP_H
#define _HTTP_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

struct HTTPResponse
{
    bool success;
    int statusCode;
    String body;
};

class HTTP
{
public:
    static HTTPResponse Post(const String &endpoint, const String &payload)
    {
        WiFiClient wifi;
        HTTPClient http;
        if (http.begin(wifi, endpoint))
        {
            http.addHeader("Content-Type", "application/json");
            int statusCode = http.POST(payload);
            if (statusCode > 0)
            {
                String body = http.getString();
                return {true, statusCode, body};
            }
            else
            {
                return {false, statusCode};
            }
            http.end();
        }
        return {false};
    }
};

#endif