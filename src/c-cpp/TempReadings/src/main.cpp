#include <WiFi.h>
#include <HTTPClient.h>

#include "Arduino.h"
#include "esp_wifi.h"
#include "soc/sens_reg.h"

#define SSID        "fkplatedev24"
#define PASSWORD    "20fkpr18"
#define HOST        "192.168.43.246"
#define PORT        5000
#define TEMP_ENDP   "temp"
#define WEIGHT_ENDP "weight"
#define MESSAGE     "{\n\t\"msg\": %d}"
#define ENDP_BKBN   "http://%s:%d/%s" // host:port/manager


enum class LedColour : uint8_t { Blue = D10, Green = D11 };


WiFiClient espClient;
HTTPClient httpClient;

LedColour led_colour;

char json_reading[256]; // 1 Mb
char complete_endpoint[256];

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

int read_temp() {
  WRITE_PERI_REG(SENS_SAR_READ_CTRL2_REG, read_ctl2);
  return analogRead(A0);
}

int read_weight() {
  WRITE_PERI_REG(SENS_SAR_READ_CTRL2_REG, read_ctl2);
  return analogRead(A1);
}

void prepare_temp () {
  sprintf(json_reading, MESSAGE, read_temp());
  sprintf(complete_endpoint, ENDP_BKBN, HOST, PORT, TEMP_ENDP);
}

void prepare_weight () {
  sprintf(json_reading, MESSAGE, read_weight());
  sprintf(complete_endpoint, ENDP_BKBN, HOST, PORT, WEIGHT_ENDP);
}

void send_data () {
  httpClient.begin(complete_endpoint);
  httpClient.addHeader("Content-Type", "application/json");

  httpClient.POST(json_reading);
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

  led_colour = (rand() % 10 > 5) ? LedColour::Green : LedColour::Blue; 
}

void loop() {
  switch_led();
  prepare_temp();
  send_data();
  delay(1000);

  switch_led();
  prepare_weight();
  send_data();
  delay(1000);
}