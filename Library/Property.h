#include "Request.h"

class Property {
  private:
    String name;

    std::function<void(const Request&)> handler;

  public:

    Property(String name) {
      this->name = name;
    }

    Property& intVal(String name, int min, int max) {
      return *this;
    }

    Property &enumVal(String name, String values[]) {
      return *this;
    }

    void handle(std::function<void(const Request&)> handler) {
      this->handler = handler;    
    }
    
};
