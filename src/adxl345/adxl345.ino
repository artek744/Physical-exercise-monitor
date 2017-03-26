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

void setupDevice(const Adxl345 *device, AxisesData offset, AxisesData gain) {
  device->powerOn();
  device->setRangeSetting(4);            
  device->calibrate(offset, gain);
}

AxisesData createCalibrationStructure(float x, float y, float z)
{
  AxisesData calibrationStructure;
  calibrationStructure.x = x;
  calibrationStructure.y = y;
  calibrationStructure.z = z;

  return calibrationStructure;
}

void setup()
{
  Wire.begin();
  Serial.begin(9600);  // start serial for output
  Serial.println("\n\nSTART:\n");

  AxisesData offsetDev1 = createCalibrationStructure(DEV1_OFFSET_X, DEV1_OFFSET_Y, DEV1_OFFSET_Z);
  AxisesData gainDev1 = createCalibrationStructure(DEV1_GAIN_X, DEV1_GAIN_Y, DEV1_GAIN_Z);
  
  AxisesData offsetDev2 = createCalibrationStructure(DEV2_OFFSET_X, DEV2_OFFSET_Y, DEV2_OFFSET_Z);
  AxisesData gainDev2 = createCalibrationStructure(DEV2_GAIN_X, DEV2_GAIN_Y, DEV2_GAIN_Z);
  
  AxisesData offsetDev3 = createCalibrationStructure(DEV3_OFFSET_X, DEV3_OFFSET_Y, DEV3_OFFSET_Z);
  AxisesData gainDev3 = createCalibrationStructure(DEV3_GAIN_X, DEV3_GAIN_Y, DEV3_GAIN_Z);
  
  AxisesData offsetDev4 = createCalibrationStructure(DEV4_OFFSET_X, DEV4_OFFSET_Y, DEV4_OFFSET_Z);
  AxisesData gainDev4 = createCalibrationStructure(DEV4_GAIN_X, DEV4_GAIN_Y, DEV4_GAIN_Z);
  
  tcaselect(0);
  setupDevice(&adxlDev1, offsetDev1, gainDev1);
  tcaselect(1);
  setupDevice(&adxlDev2, offsetDev2, gainDev2);
  tcaselect(2);
  setupDevice(&adxlDev3, offsetDev3, gainDev3);
  tcaselect(3);
  setupDevice(&adxlDev4, offsetDev4, gainDev4);

}

void printData(int devNumber, AxisesData data) {
//  Serial.print("DEV:"); Serial.print(devNumber); Serial.print(" "); Serial.print (data.x); Serial.print(" "); Serial.print(data.y); Serial.print(" "); Serial.print(data.z);
//  
  int pitch = (atan2(data.x,sqrt(data.y*data.y+data.z*data.z)) * 180.0) / PI;
  int roll = (atan2(data.y,(sqrt(data.x*data.x+data.z*data.z))) * 180.0) / PI;
  Serial.print("DEV:"); Serial.print(devNumber); Serial.print("    PITCH: "); Serial.print(pitch); Serial.print("    ROLL: "); Serial.print(roll);
  Serial.println();
}

void showAccelCalibratedData(Adxl345 device, int tcaSelectNumber) {
  AxisesData data;
  tcaselect(tcaSelectNumber);
  data = device.readAccelCalibrated();
  printData(tcaSelectNumber+1, data);
}

void loop()
{
  showAccelCalibratedData(adxlDev1, 0);
  showAccelCalibratedData(adxlDev2, 1);
  showAccelCalibratedData(adxlDev3, 2);
  showAccelCalibratedData(adxlDev4, 3);
  
  delay(1500);

}





