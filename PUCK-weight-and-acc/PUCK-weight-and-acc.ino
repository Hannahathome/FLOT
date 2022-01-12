//*********************************************************************************************************
// Hannah van Iterson 2021
//
// This is the code for Puck, the sensor of the sytem
//
// Inspiration taken from:
// www.ladyada.net/learn/sensors/fsr.html
// https://randomnerdtutorials.com/esp32-mpu-6050-web-server/
// https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/ by Rui Santos
// Time-Series Signal Processing by Rong-Hao Liang: r.liang@tue.nl
//*********************************************************************************************************


#include "./fsr.h"
#include "./mpu.h"
#include "./wireless.h"

Message myData;

//----------------------------------------------------------------------------------------------------------------//
void setup() {
  Serial.begin(115200);  //serial

  initWireless();
  initMPU();
}

void loop() {
  Serial.flush();  //Flush the serial buffer

  readMPU();

  //------------------------SENDING THE VALUES TO BLUETOOTH PEER---------------------------//
  myData.BaccX = getSmoothMPUX();
  myData.BaccY = getSmoothMPUY();
  myData.BaccZ = getSmoothMPUZ();
  myData.BgyroX = getCorrectedGyroX();
  myData.BgyroY = getCorrectedGyroY();
  myData.BgyroZ = getCorrectedGyroZ();
  myData.Bweight = getFSRWeightInGrams();
  Serial.println(getFSRWeightInGrams());

  sendWirelessMessage(myData);
  delay(100);
}
