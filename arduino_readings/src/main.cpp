#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "Arduino.h"
#include "esp_wifi.h"
#include "soc/sens_reg.h"

#define SSID        "RedDeRober"
#define PASSWORD    "RDR23Pass" //"a13c5ba1f5d3"
#define HOST        "192.168.43.246" // this host
#define PORT        5000
#define MSG_ENDP    "temp"
#define ID_ENDP     "temp/new"
#define MESSAGE     "{\"msg\": %f, \"id\": %d}"
#define ENDP_BKBN   "http://%s:%d/%s" // host:port/manager

constexpr int32_t   max_reading = 4096;
constexpr uint8_t   temp_pin    = A0;
constexpr uint8_t   weight_pin  = A1;

enum class LedColour 
  : uint8_t 
{ 
  Blue  = D10, 
  Green = D11 
};

WiFiClient espClient;
HTTPClient httpClient;

LedColour led_colour;

uint32_t device_id;
uint64_t read_ctl2;


void wifi_connect() {

  read_ctl2 = READ_PERI_REG(SENS_SAR_READ_CTRL2_REG);

  Serial.println();
  Serial.print("WiFi connecting to ");
  Serial.print(SSID);

  esp_wifi_start();
  WiFi.begin(SSID, PASSWORD);
  delay(500);

  while (!WiFi.isConnected()) {
    digitalWrite(D9, 255);
    delay(1000);
    Serial.print(".");
    digitalWrite(D9, 0);
    delay(1000);
  }

  Serial.println("-> connected");
  Serial.print("node IP address: ");
  Serial.println(WiFi.localIP());
}

int read(uint8_t pin) {
  WRITE_PERI_REG(SENS_SAR_READ_CTRL2_REG, read_ctl2);
  return max_reading - analogRead(pin);
}

void send (const char* server_endpoint, float reading) {

  char json_reading[256];
  char complete_endpoint[128];

  sprintf(complete_endpoint, ENDP_BKBN, HOST, PORT, server_endpoint);
  sprintf(json_reading, MESSAGE, reading, device_id);

  httpClient.begin(complete_endpoint);
  httpClient.addHeader("Content-Type", "application/json");

  Serial.printf("Sending: %s\nTo: %s\n", json_reading, complete_endpoint);

  httpClient.POST(json_reading);
}

float transform_boundary(int reading, float lower_bound, float upper_bound) {
  return ((reading / (float) max_reading) * upper_bound) + lower_bound; // standard normalization function
}

void switch_led () {
  digitalWrite((uint8_t) led_colour, 0);

  switch (led_colour) {
    case LedColour::Green: led_colour = LedColour::Blue; break;
    case LedColour::Blue: led_colour = LedColour::Green; break;
  }

  digitalWrite((uint8_t) led_colour, 255);
}

void shutdown_rgb_led () {
  pinMode(D9, OUTPUT);
  pinMode(D10, OUTPUT);
  pinMode(D11, OUTPUT);

  digitalWrite(D9, 0);
  digitalWrite(D10, 0);
  digitalWrite(D11, 0);
}

void get_device_id() {
  char complete_endpoint[128];
  
  sprintf(complete_endpoint, ENDP_BKBN, HOST, PORT, ID_ENDP);

  httpClient.begin(complete_endpoint);

  if (httpClient.GET() == 200) {

    String payload = httpClient.getString();
    Serial.println(payload);

    DynamicJsonDocument json(50);
    
    DeserializationError error = deserializeJson(json, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    Serial.println(json["id"].as<int>());
    device_id = json["id"].as<uint32_t>();
  } else {
    exit(EXIT_FAILURE);
  }
}

void setup() {
  Serial.begin(9600);

  shutdown_rgb_led();
  wifi_connect();
  get_device_id();

  led_colour = (rand() % 2) ? LedColour::Green : LedColour::Blue; 
}

void loop() {

  const int temp_reading = read(temp_pin);
  const float fair_temp_reading = transform_boundary(temp_reading, 0.0f, 55.0f);

  send(MSG_ENDP, fair_temp_reading);
  delay(1000);
  switch_led();
}