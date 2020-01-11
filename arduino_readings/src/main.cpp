#include <WiFi.h>
#include <HTTPClient.h>

#include "Arduino.h"
#include "esp_wifi.h"
#include "soc/sens_reg.h"

#define SSID        "RedDeRober"
#define PASSWORD    "RDR23Pass"
#define HOST        "192.168.1.150"
#define PORT        5000
#define TEMP_ENDP   "temp"
#define WEIGHT_ENDP "weight"
#define MESSAGE     "{\"msg\": %f}"
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

void send (char* server_endpoint, float reading) {

  char json_reading[256];
  char complete_endpoint[128];

  sprintf(complete_endpoint, ENDP_BKBN, HOST, PORT, server_endpoint);
  sprintf(json_reading, MESSAGE, reading);

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

void setup() {
  Serial.begin(9600);

  shutdown_rgb_led();
  wifi_connect();

  led_colour = (rand() % 2) ? LedColour::Green : LedColour::Blue; 
}

void loop() {

  /** Forma entendible **/
  const int temp_reading = read(temp_pin);
  const float fair_temp_reading = transform_boundary(temp_reading, 0.0f, 55.0f);

  send(TEMP_ENDP, fair_temp_reading);
  delay(1000);
  switch_led();

  /** Forma concisa **/
  send(WEIGHT_ENDP, transform_boundary(read(weight_pin), 10.0f, 200.0f));
  delay(1000);
  switch_led();
}