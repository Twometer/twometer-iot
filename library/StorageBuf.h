#ifndef _STORAGE_BUF_H
#define _STORAGE_BUF_H

#include <EEPROM.h>
#include <Arduino.h>

class StorageBuf
{

private:
    int offset = 0;

public:
    void writeString(String str)
    {
        writeInt(str.length() + 1);
        const byte *data = (const byte *)(const void *)str.c_str();
        int byteSize = sizeof(char) * str.length();
        for (int i = 0; i < byteSize; i++)
            writeByte(data[i]);
        writeByte(0);
    }

    String readString()
    {
        int len = readInt();
        int byteSize = sizeof(char) * len;
        char data[len];
        for (int i = 0; i < byteSize; i++)
            data[i] = readByte();
        return String(data);
    }

    template <typename T>
    void writeAny(T &t)
    {
        EEPROM.put(offset, t);
        offset += sizeof(T);
    }

    template <typename T>
    T readAny()
    {
        T t;
        EEPROM.get(offset, t);
        offset += sizeof(T);
        return t;
    }

    inline int readInt()
    {
        return readAny<int>();
    }

    inline void writeInt(int i)
    {
        writeAny(i);
    }

    byte readByte()
    {
        byte b = EEPROM.read(offset);
        offset++;
        return b;
    }

    void writeByte(byte b)
    {
        EEPROM.write(offset, b);
        offset++;
    }

    void save()
    {
        EEPROM.commit();
    }
};

#endif