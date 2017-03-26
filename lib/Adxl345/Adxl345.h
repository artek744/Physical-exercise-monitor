#ifndef ADXL345_h
#define ADXL345_h
 
#include <Wire.h>
#include "Arduino.h"
 
#define ADXL345_TO_READ 6
 
#define ADXL345_POWER_CTL 0x2d
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0 0x32
#define ADXL345_DEVICE_ADDRESS 0x53 

struct AxisesData
{
   float x;
   float y;
   float z;
};
 
class Adxl345
{

public:
   Adxl345();
   void powerOn();
   void setRangeSetting(int val);
   void calibrate(const AxisesData &offset, const AxisesData &gain);
   AxisesData readAccelRaw();
   AxisesData readAccelCalibrated();
 
private:
   void readFrom(byte address, int num, byte buff[]);
   void writeTo(byte address, byte val);

   float offsetX;
   float offsetY;
   float offsetZ;
   float gainX;
   float gainY;
   float gainZ;      
     
};
 
#endif
