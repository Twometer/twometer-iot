#ifndef UTILS_H
#define UTILS_H

#include <ESP8266HTTPClient.h>

/**
   Generate a random key of given length
*/
String GenerateKey(int len) {
  String output;
  for (int i = 0; i < len; i++) {
    char c = 'a' + random('z' - 'a');
    output += c;
  }
  return output;
}

/**
   Remove an item from a vector
*/
template <typename T>
void remove(std::vector<T>& vec, T& v) {
  typename std::vector<T>::iterator it = vec.begin();
  while (it != vec.end()) {
    if (*it == v)
      it = vec.erase(it);
    else ++it;
  }
}

/**
   Do an HTTP request
*/
String request(String url) {
  WiFiClient wifi;
  HTTPClient http;
  http.begin(wifi, url);

  int code = http.GET();
  if (code == 0) return "";
  String payload = http.getString();
  http.end();
  return payload;
}

String requestWithTimeout(String url, int timeout)
{
  WiFiClient wifi;
  HTTPClient http;
  http.begin(wifi, url);
  http.setTimeout(timeout);

  int code = http.GET();
  Serial.println("Code: " + String(code));
  if (code <= 0) return "";
  String payload = http.getString();
  http.end();
  return payload;
}

#endif
