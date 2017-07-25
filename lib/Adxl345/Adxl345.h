#ifndef ADXL345_h
#define ADXL345_h

#include <Wire.h>
#include "Arduino.h"

#define ADXL345_TO_READ 6

#define ADXL345_POWER_CTL 0x2d
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0 0x32
#define ADXL345_DEVICE_ADDRESS 0x53
#define DATA_IS_NOT_SET -999.0

typedef struct Axes
{
	float x = DATA_IS_NOT_SET;
	float y = DATA_IS_NOT_SET;
	float z = DATA_IS_NOT_SET;
} Axes;

typedef struct Rotate
{
	int pitch = DATA_IS_NOT_SET;
	int roll = DATA_IS_NOT_SET;
} Rotate;

typedef struct AdxlData
{
	Axes axes;
	Rotate rotate;
} AdxlData;

class Adxl345
{

public:
	Adxl345();
	void powerOn();
	void setRangeSetting(int val);
	void calibrate(const Axes &offset, const Axes &gain);
	AdxlData readData();
	void updateData();
	AdxlData getData();

private:
	void readFrom(byte address, int num, byte buff[]);
	void writeTo(byte address, byte val);
	Axes calculateCalbiratedAxes(Axes rawAxes);
	Rotate  calculateRotate(Axes calibratedAxes);

	AdxlData readRawData();
	AdxlData readCalibratedData();
	AdxlData (Adxl345::*dataReader)();
	AdxlData data;

	float offsetX;
	float offsetY;
	float offsetZ;
	float gainX;
	float gainY;
	float gainZ;

};

#endif
