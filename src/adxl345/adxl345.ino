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

/* -------------------------- */

Adxl345 adxlDev1 = Adxl345(device1Address);
Adxl345 adxlDev2 = Adxl345(device1Address);
Adxl345 adxlDev3 = Adxl345(device1Address);
Adxl345 adxlDev4 = Adxl345(device1Address);

void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setup()
{
  Serial.begin(9600);  // start serial for output
  Serial.println("\n\nSTART:\n");

  AccelCalibration offsetDev1;
  AccelCalibration gainDev1;
  AccelCalibration offsetDev2;
  AccelCalibration gainDev2;
  AccelCalibration offsetDev3;
  AccelCalibration gainDev3;
  AccelCalibration offsetDev4;
  AccelCalibration gainDev4;
  
  offsetDev1.x = DEV1_OFFSET_X;
  offsetDev1.y = DEV1_OFFSET_Y ;
  offsetDev1.z = DEV1_OFFSET_Z;
  gainDev1.x = DEV1_GAIN_X;
  gainDev1.y = DEV1_GAIN_Y;
  gainDev1.z = DEV1_GAIN_Z;

  offsetDev2.x = DEV2_OFFSET_X;
  offsetDev2.y = DEV2_OFFSET_Y ;
  offsetDev2.z = DEV2_OFFSET_Z;
  gainDev2.x = DEV2_GAIN_X;
  gainDev2.y = DEV2_GAIN_Y;
  gainDev2.z = DEV2_GAIN_Z;

  offsetDev3.x = DEV3_OFFSET_X;
  offsetDev3.y = DEV3_OFFSET_Y ;
  offsetDev3.z = DEV3_OFFSET_Z;
  gainDev3.x = DEV3_GAIN_X;
  gainDev3.y = DEV3_GAIN_Y;
  gainDev3.z = DEV3_GAIN_Z;

  offsetDev4.x = DEV4_OFFSET_X;
  offsetDev4.y = DEV4_OFFSET_Y ;
  offsetDev4.z = DEV4_OFFSET_Z;
  gainDev4.x = DEV4_GAIN_X;
  gainDev4.y = DEV4_GAIN_Y;
  gainDev4.z = DEV4_GAIN_Z;
    
  adxlDev1.powerOn();
  adxlDev1.setRangeSetting(4);            
  adxlDev1.calibrate(offsetDev4, gainDev4);

}


void loop()
{
  AccelCalibration data, data2;
  tcaselect(7);
  data2 = adxlDev1.readAccelCalibrated();
  Serial.print("DEV2: "); Serial.print (data2.x); Serial.print(" "); Serial.print(data2.y); Serial.print(" "); Serial.print(data2.z);
  Serial.println();

}





