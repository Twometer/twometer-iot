#ifndef _MESSAGE_H
#define _MESSAGE_H

class Message
{
private:
    String type;
    String data;
    int index = 0;

public:
    Message()
    {
    }

    Message(String type)
        : type(type)
    {
    }

    void parse(String data)
    {
        this->data = data;
        this->type = readString();
    }

    const String &getType() const
    {
        return type;
    }

    String toString() const
    {
        return type + ":" + data;
    }

    bool readBool()
    {
        return readString() == "true";
    }

    String readString()
    {
        String str{};
        while (!eof())
        {
            char c = data[index];
            index++;
            if (c == ':')
                break;
            else
                str += c;
        }
        return str;
    }

    Color readColor()
    {
        return {readInt(), readInt(), readInt()};
    }

    int readInt()
    {
        return readString().toInt();
    }

    void writeInt(int i)
    {
        writeSeparator();
        data += i;
    }

    void writeString(String str)
    {
        writeSeparator();
        data += str;
    }

private:
    bool eof() const
    {
        return index >= data.length();
    }

    bool writeSeparator()
    {
        if (data.length() != 0)
            data += ":";
    }
};

#endif