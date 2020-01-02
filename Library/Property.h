#include "Request.h"

class Property {
  public:
    String name;

    String dataType;
  
    std::function<bool(const Request&)> handler;

    Property(String name, String dataType) {
      this->name = name;
      this->dataType = dataType;
    }

    void handle(std::function<bool(const Request&)> handler) {
      this->handler = handler;
    }
};
