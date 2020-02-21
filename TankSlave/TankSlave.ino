#include <esp_now.h>
#include <WiFi.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define TFT_CS         5
#define TFT_RST        4
#define TFT_DC         2
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0x96, 0x18, 0x00};

typedef struct struct_message {
  uint8_t photoBytes[160]; // ESP Now supports max of 250 bytes at each packet
} struct_message;

struct_message incomingReadings;
struct_message incomingData;

uint8_t r_color = 50;
uint8_t g_color = 50;
uint8_t b_color = 50;
unsigned int pixelCounter = 0;
int rowCounter = 0;
unsigned int zeroCounter = 0;
// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.println("!");

  pixelCounter = 0;
  if (zeroCounter > 100) {
    zeroCounter = 0;
    rowCounter = 0;
    pixelCounter = 0;
  }

  for (uint8_t i = 0; i < 160; i++) {

    if (incomingReadings.photoBytes[i] == 0) {
      zeroCounter++;
    }

    if (rowCounter >= 159) {
      rowCounter = 0;
      pixelCounter = 0;
    }
    uint16_t RGB565 = (((incomingReadings.photoBytes[i] & 0b11111000) << 8) + ((incomingReadings.photoBytes[i] & 0b11111100) << 3) + (incomingReadings.photoBytes[i] >> 3));
    tft.drawPixel(pixelCounter, rowCounter, RGB565);
    pixelCounter += 1;
  }
  rowCounter++;
  Serial.println("");
  Serial.println("?");


  //  tft.setCursor(0, 0);
  //  tft.setTextColor(0x4228);
  //  tft.setTextWrap(true);
  //  tft.print("Emre Sahin, Hello World!");
}

void setup() {
  Serial.begin(115200);

  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

}


















/*#include  <SPI.h>
  #include "nRF24L01.h"
  #include "RF24.h"
  char msg[128] = "";
  RF24 radio(12, 14, 26, 25, 27);
  //const uint64_t pipe = 0xE8E8F0F0E1LL;

  void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  radio.begin();
  radio.setChannel(2);
  radio.setPayloadSize(7);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(0xE8E8F0F0E1LL);
  }
  bool escape = false;
  void loop() {
  if (Serial2.available()) {
    char serialData = char(Serial2.read());
    if (serialData == '!') { //take photo
      Serial.println("Retrieving Photo Data..");
      while (1) {
        for (uint8_t i = 0; i < 128; i++) {
          serialData = char(Serial2.read());
          if (serialData == '?' || serialData == '⸮') { //end of data or error
            Serial.println("AAAAAAAAAAAA");
            escape = true;
            break;
          }
          msg[i] = serialData;
          Serial.print(serialData);
        }
        if (escape) {
          escape = false;
          break;
        }
        delay(100);
        radio.write(msg, 128);

      }
    }
  }
  //delay(3000);
  }
*/
