#include <Adxl345.h>
#include <math.h>

#define device1Address 0x1D
#define device2Address 0x53

Adxl345 adxlDev1 = Adxl345(device1Address);
Adxl345 adxlDev2 = Adxl345(device2Address);

int redLed = 13;
int yellowLed = 12;
int greenLed = 11;

#define dev1OffsetX -64.00
#define dev1OffsetY -67.00
#define dev1OffsetZ 172.00
#define dev1GainX 131.50
#define dev1GainY 131.00
#define dev1GainZ 137.50

#define dev2OffsetX 12.00
#define dev2OffsetY -13.00
#define dev2OffsetZ -19.50
#define dev2GainX 132.00
#define dev2GainY 132.00
#define dev2GainZ 127.50 
/* -------------------------- */


void setup()
{
  Serial.begin(9600);  // start serial for output

  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  digitalWrite(redLed, LOW);
  digitalWrite(yellowLed, LOW);
  digitalWrite(greenLed, LOW);
  
  AccelCalibration offsetDev1;
  AccelCalibration gainDev1;
  
  offsetDev1.x = dev1OffsetX;
  offsetDev1.y = dev1OffsetY;
  offsetDev1.z = dev1OffsetZ;
  gainDev1.x = dev1GainX;
  gainDev1.y = dev1GainY;
  gainDev1.z = dev1GainZ;
    
  adxlDev1.powerOn();
  adxlDev1.setRangeSetting(4);            
  adxlDev1.calibrate(offsetDev1, gainDev1);


  AccelCalibration offsetDev2;
  AccelCalibration gainDev2;

  offsetDev2.x = dev2OffsetX;
  offsetDev2.y = dev2OffsetY;
  offsetDev2.z = dev2OffsetZ;
  gainDev2.x = dev2GainX;
  gainDev2.y = dev2GainY;
  gainDev2.z = dev2GainZ;

  adxlDev2.powerOn();
  adxlDev2.setRangeSetting(4);
  adxlDev2.calibrate(offsetDev2, gainDev2);
}


void loop()
{
  AccelCalibration data, data2;

  data = adxlDev1.readAccelCalibrated();
  Serial.print("DEV1: "); Serial.print (data.x); Serial.print(" "); Serial.print(data.y); Serial.print(" "); Serial.print(data.z);
  Serial.println();
  
  data2 = adxlDev2.readAccelCalibrated();
  Serial.print("DEV2: "); Serial.print (data2.x); Serial.print(" "); Serial.print(data2.y); Serial.print(" "); Serial.print(data2.z);
  Serial.println();

  Serial.print("Different:"); Serial.print (abs(data2.x-data.x)); Serial.print(" "); Serial.print(abs(data2.y-data.y)); Serial.print(" "); Serial.print(abs(data2.z-data.z));
  Serial.println();
  Serial.println();

  if((abs(data2.x-data.x)) > 0.3) {
    digitalWrite(redLed, HIGH);
  }
  else {
    digitalWrite(redLed, LOW);
  }

  if((abs(data2.y-data.y)) > 0.3) {
    digitalWrite(yellowLed, HIGH);
  }
  else {
    digitalWrite(yellowLed, LOW);
  }

  if((abs(data2.z-data.z)) > 0.3) {
    digitalWrite(greenLed, HIGH);
  }
  else {
    digitalWrite(greenLed, LOW);
  }
 
  delay(200);
}





