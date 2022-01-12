#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>
#include <esp_now.h>

#include "./data.h"
//BLUETOOTH SENDER CONNECTION --> using ESP_NOW
uint8_t broadcastAddress[] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX};  // REPLACE WITH YOUR RECEIVER MAC Address --> you can use the first part this tutorial: https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail"); //check
}

static void initWireless() {
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully initiated, we will register for Send CB to get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer --> so the one you are going to be SENDING data to
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  WiFi.mode(WIFI_OFF);
};

static void sendWirelessMessage(const Message &myData) {
  // Send Message via ESP-NOW
  WiFi.mode(WIFI_STA);
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  WiFi.mode(WIFI_OFF);

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
};
