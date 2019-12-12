const char* WIFI_NAME_PAIR = "Twometer IoT Pair"; // The Pairing WiFi
const char* WIFI_NAME_CTRL = "Twometer IoT"; // The Control WiFi

class WiFiController {
public:
  void initialize();
  void begin_pair();
  void end_pair();
  const char* get_key();
};
