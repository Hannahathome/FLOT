#include <Wire.h>

int fsrPin = 12;            // the Force Sensitive Resistor and 10K pulldown are connected to D12 --> 18
float fsrReading;           // the analog reading from the FSR resistor divider
int fsrWeightSamples = 10;  // average of this number of readings is taken

int getFSRWeightInGrams() {
  
  float fsrAverage = 0;             // it needs to be reset
  
  for (int i = 0; i < fsrWeightSamples; ++i) {  // taking the average
    fsrReading = analogRead(fsrPin);  // Get current weight
    fsrAverage += fsrReading / fsrWeightSamples;
    delay(10);
  }

  // getting the weight instead of the raw sensor reading
  float p1 = 0.00000005;  //5E-08
  float p2 = 0.0002;
  float p3 = 0.326;
  float p4 = 178.81; // Weight of the cup
  return (p1 * (fsrAverage * fsrAverage * fsrAverage)) - (p2 * (fsrAverage * fsrAverage)) + (p3 * fsrAverage) - p4;  // based on formfitted formula
}
