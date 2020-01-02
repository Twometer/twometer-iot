#ifndef UTILS_H
#define UTILS_H

const char* GenerateKey(int len) {
  char* key = new char[len];
  for (int i = 0; i < len; i++) {
    char c = 'a' + rand() * ('z' - 'a');
    key[i] = c;
  }
  return key;
}

#endif
