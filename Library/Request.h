#include <Arduino.h>

struct Arg {
  String key;
  JsonVariant val;

  Arg(String key, JsonVariant val) {
    this->key = key;
    this->val = val;
  }
};

class Request {
  private:
    std::vector<Arg> args;

    JsonVariant findArg(String key) const {
      for (int i = 0; i < args.size(); i++)
      {
        const Arg& arg = args.at(i);
        if (arg.key == key)
          return arg.val;
      }

      throw std::invalid_argument("unknown arg");
    }
    
  public:
    Request(std::vector<Arg> args) {
      this->args = args;
    }

    int intVal(String key, int min, int max) const {
      int r = findArg(key).as<int>();
      if (r < min || r > max)
        throw std::invalid_argument("bad arg");
      return r;
    }

    String enumVal(String key, const std::vector<String> & values) const {
      String val = findArg(key).as<String>();
      for (String str : values)
        if (str == val)
          return val;
      throw std::invalid_argument("bad arg");
    }

};
