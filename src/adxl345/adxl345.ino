#include <Adxl345.h>
#include <math.h>
 
#define device1Address 0x53
#define TCAADDR 0x70

#define DEV1_OFFSET_X -63.50
#define DEV1_OFFSET_Y -66.50
#define DEV1_OFFSET_Z 172.00
#define DEV1_GAIN_X 131.50
#define DEV1_GAIN_Y 131.00
#define DEV1_GAIN_Z 138.00

#define DEV2_OFFSET_X -0.50
#define DEV2_OFFSET_Y -11.00
#define DEV2_OFFSET_Z -5.50
#define DEV2_GAIN_X 127.50
#define DEV2_GAIN_Y 128.00
#define DEV2_GAIN_Z 123.50

#define DEV3_OFFSET_X -1.50
#define DEV3_OFFSET_Y -1.50
#define DEV3_OFFSET_Z 3.50
#define DEV3_GAIN_X 130.50
#define DEV3_GAIN_Y 131.50
#define DEV3_GAIN_Z 128.00

#define DEV4_OFFSET_X 12.00
#define DEV4_OFFSET_Y -13.50
#define DEV4_OFFSET_Z -18.00
#define DEV4_GAIN_X 132.00
#define DEV4_GAIN_Y 132.00
#define DEV4_GAIN_Z 127.50

Adxl345 adxlDev1 = Adxl345();
Adxl345 adxlDev2 = Adxl345();
Adxl345 adxlDev3 = Adxl345();
Adxl345 adxlDev4 = Adxl345();

void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setupDevice(const Adxl345 *device, Axes offset, Axes gain) {
  device->powerOn();
  device->setRangeSetting(4);            
  device->calibrate(offset, gain);
}

Axes createCalibrationStructure(float x, float y, float z)
{
  Axes calibrationStructure;
  calibrationStructure.x = x;
  calibrationStructure.y = y;
  calibrationStructure.z = z;

  return calibrationStructure;
}

int ledPin = 8;

void setup()
{
  pinMode(ledPin, OUTPUT);
  
  Wire.begin();
  Serial.begin(9600);  // start serial for output
  Serial.println("\n\nSTART:\n");

  Axes offsetDev1 = createCalibrationStructure(DEV1_OFFSET_X, DEV1_OFFSET_Y, DEV1_OFFSET_Z);
  Axes gainDev1 = createCalibrationStructure(DEV1_GAIN_X, DEV1_GAIN_Y, DEV1_GAIN_Z);
  
  Axes offsetDev2 = createCalibrationStructure(DEV2_OFFSET_X, DEV2_OFFSET_Y, DEV2_OFFSET_Z);
  Axes gainDev2 = createCalibrationStructure(DEV2_GAIN_X, DEV2_GAIN_Y, DEV2_GAIN_Z);
  
//  Axes offsetDev3 = createCalibrationStructure(DEV3_OFFSET_X, DEV3_OFFSET_Y, DEV3_OFFSET_Z);
//  Axes gainDev3 = createCalibrationStructure(DEV3_GAIN_X, DEV3_GAIN_Y, DEV3_GAIN_Z);
//  
//  Axes offsetDev4 = createCalibrationStructure(DEV4_OFFSET_X, DEV4_OFFSET_Y, DEV4_OFFSET_Z);
//  Axes gainDev4 = createCalibrationStructure(DEV4_GAIN_X, DEV4_GAIN_Y, DEV4_GAIN_Z);
  
  tcaselect(0);
  setupDevice(&adxlDev1, offsetDev1, gainDev1);
  tcaselect(1);
  setupDevice(&adxlDev2, offsetDev2, gainDev2);
}

void showData(AdxlData data, int devNumber) {
  Serial.print("DEV:"); Serial.println(devNumber); 
  Serial.print("PITCH: "); Serial.print(data.rotate.pitch); Serial.print("    ROLL: "); Serial.println(data.rotate.roll);
//  Serial.print("X:"); Serial.print(data.axes.x); Serial.print("  Y:"); Serial.print(data.axes.y); Serial.print("  Z:"); Serial.print(data.axes.z); 
  Serial.println();
}

bool isCorrect(Rotate rotate1, Rotate rotate2)
{
  if(abs(rotate1.roll - (-1)*rotate2.roll) > 30) {
    return false ;
  }

  if(abs(rotate1.pitch - rotate2.pitch) > 30) {
    return false ;
  }

  return true;
}

void loop()
{
  tcaselect(0);
  adxlDev1.updateData();
  tcaselect(1);
  adxlDev2.updateData();
  
  showData(adxlDev1.getData(), 0);
  showData(adxlDev2.getData(), 1);

  if(isCorrect(adxlDev1.getData().rotate, adxlDev2.getData().rotate)) {
    digitalWrite(ledPin, LOW);
  }
  else {
    digitalWrite(ledPin, HIGH);
  }

  Serial.println("\n");
//  delay(2000);

}





