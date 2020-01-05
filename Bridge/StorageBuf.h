#ifndef storagebuf_h
#define storagebuf_h

#include <EEPROM.h>

class StorageBuf {

  private:
    int eeprom_offset = 0;

  public:
    void write_string(String str) {
      write_int(str.length() + 1);
      const byte* data = (const byte*)(const void*)str.c_str();
      int byte_size = sizeof(char) * str.length();
      for (int i = 0; i < byte_size; i++)
        write_byte(data[i]);
      write_byte(0);
    }

    String read_string() {
      int len = read_int();
      int byte_size = sizeof(char) * len;
      char* data = new char[len];
      for (int i = 0; i < byte_size; i++)
        data[i] = read_byte();
      return String(data);
    }

    template<typename T>
    void write_any(T& t) {
      EEPROM.put(eeprom_offset, t);
      eeprom_offset += sizeof(T);
    }

    template<typename T>
    T read_any() {
      T t;
      EEPROM.get(eeprom_offset, t);
      eeprom_offset += sizeof(T);
      return t;
    }

    inline int read_int() {
      return read_any<int>();
    }

    inline void write_int(int i) {
      write_any(i);
    }

    byte read_byte() {
      byte b = EEPROM.read(eeprom_offset);
      eeprom_offset ++;
      return b;
    }

    void write_byte(byte b) {
      EEPROM.write(eeprom_offset, b);
      eeprom_offset ++;
    }
};

#endif
