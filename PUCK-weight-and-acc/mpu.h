// INCLUDE SPECIFIC SENSOR LIBRARIES --> install via library installer
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// CREATING SENSOR OBJECTS
Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;  //accelero, gyroscope, temperature
float gyroXerror = 0.07;
float gyroYerror = 0.03;
float gyroZerror = 0.01;

//Smoothing
#define WINDOW_SIZE 5  //large window size = smoother curve because average over more points but it will run slower the more you add!
int indexX = 0;
float valueX = 0;
float sumX = 0;
float ReadingsX[WINDOW_SIZE];
float averageX = 0;

int indexY = 0;
float valueY = 0;
float sumY = 0;
float ReadingsY[WINDOW_SIZE];
float averageY = 0;

int indexZ = 0;
float valueZ = 0;
float sumZ = 0;
float ReadingsZ[WINDOW_SIZE];
float averageZ = 0;

void initMPU() {
  // start to find the MPU Chip
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");  //in case stuff fails
    while (1) {
      delay(10);
    }
  }
};

void readMPU() {
  mpu.getEvent(&a, &g, &temp);  //Read the MPU6050
};

float getSmoothMPUX() {
  sumX = sumX - ReadingsX[indexX];      // Remove the oldest entry from the sum
  valueX = a.acceleration.x;            // Read the next sensor value
  ReadingsX[indexX] = valueX;           // Add the newest reading to the window
  sumX = sumX + valueX;                 // Add the newest reading to the sum
  indexX = (indexX + 1) % WINDOW_SIZE;  // Increment the index, and wrap to 0 if it exceeds the window size
  return sumX / WINDOW_SIZE;            // Divide the sum of the window by the window size for the result
}

float getSmoothMPUY() {
  sumY = sumY - ReadingsY[indexY];      // Remove the oldest entry from the sum
  valueY = a.acceleration.y;            // Read the next sensor value
  ReadingsY[indexY] = valueY;           // Add the newest reading to the window
  sumY = sumY + valueY;                 // Add the newest reading to the sum
  indexY = (indexY + 1) % WINDOW_SIZE;  // Increment the index, and wrap to 0 if it exceeds the window size
  return sumY / WINDOW_SIZE;            // Divide the sum of the window by the window size for the result
}

float getSmoothMPUZ() {
  sumZ = sumZ - ReadingsZ[indexZ];      // Remove the oldest entry from the sum
  valueZ = a.acceleration.z;            // Read the next sensor value
  ReadingsZ[indexZ] = valueZ;           // Add the newest reading to the window
  sumZ = sumZ + valueZ;                 // Add the newest reading to the sum
  indexZ = (indexZ + 1) % WINDOW_SIZE;  // Increment the index, and wrap to 0 if it exceeds the window size
  return sumZ / WINDOW_SIZE;            // Divide the sum of the window by the window size for the result
}

float getCorrectedGyroX() {
  float gyroX = g.gyro.x;
  if (abs(g.gyro.x) > gyroXerror) {
    gyroX += g.gyro.x / 50.00;  // Correction of value when error is large
  }
  return gyroX;
}

float getCorrectedGyroY() {
  float gyroY = g.gyro.y;
  if (abs(g.gyro.y) > gyroYerror) {
    gyroY += g.gyro.y / 70.00;  // Correction of value when error is large
  }
  return gyroY;
}

float getCorrectedGyroZ() {
  float gyroZ = g.gyro.z;
  if (abs(g.gyro.z) > gyroZerror) {
    gyroZ += g.gyro.z / 90.00;  // Correction of value when error is large
  }
  return gyroZ;
}
