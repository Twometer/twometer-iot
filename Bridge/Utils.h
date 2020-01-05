#ifndef UTILS_H
#define UTILS_H

#include <ESP8266HTTPClient.h>

/**
 * Generate a random key of given length
 */
const char* GenerateKey(int len) {
  char* key = new char[len];
  for (int i = 0; i < len; i++) {
    char c = 'a' + random('z' - 'a');
    key[i] = c;
  }
  return key;
}

/**
 * Remove an item from a vector
 */
template <typename T>
void remove(std::vector<T>& vec, T& v) {
  for (int i = 0; i < vec.size(); i++) {
    T& t = vec[i];
    if (t == v) {
      vec.erase(vec.begin() + i);
      return;
    }
  }
}

/**
 * Do an HTTP request
 */
String request(String url) {
  HTTPClient http;
  http.begin(url);

  int code = http.GET();
  if (code == 0) return "";
  String payload = http.getString();
  http.end();
  return payload;
}

#endif
