#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

class TwometerIoT {
  private:
    ESP8266WebServer httpServer(80);

  public:   
    void begin() {
      httpServer.begin();
    }

    Property& prop(String name) {
      Property* property(name);

      // TODO add to vector or something
      
      return *property;
    }

    void update(){
      httpServer.handleClient();
    }
   
  
}
