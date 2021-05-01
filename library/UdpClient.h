#ifndef _UDP_CLIENT_H
#define _UDP_CLIENT_H

#include <WiFiUdp.h>
#include <stdint.h>

class UdpClient
{
private:
    WiFiUDP client{};

    IPAddress ip;
    int port;

public:
    bool begin(IPAddress ip, int port)
    {
        this->ip = ip;
        this->port = port;
        return client.begin(port);
    }

    void beginPacket()
    {
        client.beginPacket(ip, port);
    }

    template <typename T>
    void write(T data)
    {
        writeRaw((uint8_t *)(&data), sizeof(data));
    }

    void writeRaw(const void *data, int length)
    {
        client.write((uint8_t*) data, length);
    }

    void endPacket()
    {
        client.endPacket();
    }

    int readPacket(uint8_t *buffer, int maxlen)
    {
        int size = client.parsePacket();
        if (!size)
            return 0;

        if (size > maxlen)
            return -1;

        return client.read(buffer, size);
    }
};

#endif