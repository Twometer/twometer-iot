#include "Request.h"

class Property {
  public:
    String name;
  
    std::function<bool(const Request&)> handler;

    Property(String name) {
      this->name = name;
    }

    void handle(std::function<bool(const Request&)> handler) {
      this->handler = handler;
    }
};
