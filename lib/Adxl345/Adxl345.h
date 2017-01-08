#ifndef ADXL345_h
#define ADXL345_h
 
#include <Wire.h>
#include "Arduino.h"
 
#define ADXL345_TO_READ 6
 
#define ADXL345_POWER_CTL 0x2d
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0 0x32
 
struct AccelRaw
{
   int x;
   int y;
   int z;
};
 
struct AccelCalibration
{
   double x;
   double y;
   double z;
};
 
struct AccelRotation
{
   double pitch;
   double roll;
};
 
class Adxl345
{

public:
   Adxl345(int deviceAddr = 0x53);
   void powerOn();
   void setRangeSetting(int val);
   void calibrate(const AccelCalibration &offset, const AccelCalibration &gain);
   void writeTo(byte address, byte val);
   void readFrom(byte address, int num, byte buff[]);
   AccelRaw readAccelRaw();
   AccelCalibration readAccelCalibrated();

private:
   int deviceAddress;
   double offsetX;
   double offsetY;
   double offsetZ;
   double gainX;
   double gainY;
   double gainZ;      
     
};
 
#endif
