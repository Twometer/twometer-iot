#include <Arduino.h>
#include "Color.h"

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

    int intVal() const {
      return findArg("value").as<int>();
    }

    bool boolVal() const {
      return findArg("value").as<bool>();
    }

    float floatVal() const {
      return findArg("value").as<float>();
    }

    String stringVal() const {
      return findArg("value").as<String>();
    }

    ColorRgb colorRgbVal() const {
      int r = findArg("r").as<int>();
      int g = findArg("g").as<int>();
      int b = findArg("b").as<int>();
      return {r, g, b};
    }

    ColorRgbw colorRgbwVal() const {
      int r = findArg("r").as<int>();
      int g = findArg("g").as<int>();
      int b = findArg("b").as<int>();
      int w = findArg("w").as<int>();
      return {r, g, b, w};
    }

    ColorRgb2w colorRgb2wVal() const {
      int r = findArg("r").as<int>();
      int g = findArg("g").as<int>();
      int b = findArg("b").as<int>();
      int cw = findArg("cw").as<int>();
      int ww = findArg("ww").as<int>();
      return {r, g, b, cw, ww};
    }

    JsonVariant customVal(String key) const {
      return findArg(key);
    }

};
