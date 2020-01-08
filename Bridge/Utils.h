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
  for (int i = 0; i < vec.size(); i++) {
    T& t = vec[i];
    if (t == v) {
      vec.erase(vec.begin() + i);
      return;
    }
  }
}

/**
   Do an HTTP request
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
