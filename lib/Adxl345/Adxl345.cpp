#include "Adxl345.h"

Adxl345::Adxl345()
{
	this->dataReader = &Adxl345::readRawData;
}

void Adxl345::powerOn()
{
	writeTo(0x2D, 0);
	writeTo(0x2D, 16);
	writeTo(0x2D, 8);
}

void Adxl345::setRangeSetting(int val) {
	byte _s;
	byte _b;

	switch (val) {
		case 2:
			_s = B00000000;
			break;
		case 4:
			_s = B00000001;
			break;
		case 8:
			_s = B00000010;
			break;
		case 16:
			_s = B00000011;
			break;
		default:
			_s = B00000000;
	}
	readFrom(ADXL345_DATA_FORMAT, 1, &_b);
	_s |= (_b & B11101100);
	writeTo(ADXL345_DATA_FORMAT, _s);
}

void Adxl345::calibrate(const Axes &offset, const Axes &gain)
{
	this->offsetX = offset.x;
	this->offsetY = offset.y;
	this->offsetZ = offset.z;
	this->gainX = gain.x;
	this->gainY = gain.y;
	this->gainZ = gain.z;

	this->dataReader = &Adxl345::readCalibratedData;
}

void Adxl345::writeTo(byte regAddress, byte val)
{
	Wire.beginTransmission(ADXL345_DEVICE_ADDRESS);
	Wire.write(regAddress);
	Wire.write(val);
	Wire.endTransmission();
}

void Adxl345::readFrom(byte regAddress, int num, byte buff[])
{
	Wire.beginTransmission(ADXL345_DEVICE_ADDRESS);
	Wire.write(regAddress);
	Wire.endTransmission();

	Wire.beginTransmission(ADXL345_DEVICE_ADDRESS);
	Wire.requestFrom(ADXL345_DEVICE_ADDRESS, num);

	int i = 0;
	while(Wire.available())	{
		buff[i] = Wire.read();
		i++;
	}
	Wire.endTransmission();
}

Axes Adxl345::calculateCalbiratedAxes(Axes rawAxes)
{
	Axes axes;
	axes.x = (rawAxes.x - offsetX)/gainX;
	axes.y = (rawAxes.y - offsetY)/gainY;
	axes.z = (rawAxes.z - offsetZ)/gainZ;

	return axes;
}

Rotate Adxl345::calculateRotate(Axes calibratedAxes)
{
	Rotate rotate;
	rotate.pitch = (atan2(calibratedAxes.x, sqrt(calibratedAxes.y*calibratedAxes.y + calibratedAxes.z*calibratedAxes.z)) * 180.0) / PI;
	rotate.roll = (atan2(calibratedAxes.y, sqrt(calibratedAxes.x*calibratedAxes.x + calibratedAxes.z*calibratedAxes.z)) * 180.0) / PI;
	return rotate;
}

AdxlData Adxl345::readData()
{
	return (this->*dataReader)();
}

AdxlData Adxl345::readRawData()
{
	byte _buff[6];
	readFrom(ADXL345_DATAX0, ADXL345_TO_READ, _buff);

	AdxlData rawData;

	Axes rawAxes;
	rawAxes.x = (((int)_buff[1]) << 8) | _buff[0];
	rawAxes.y = (((int)_buff[3]) << 8) | _buff[2];
	rawAxes.z = (((int)_buff[5]) << 8) | _buff[4];

	rawData.axes = rawAxes;
	return rawData;
}

AdxlData Adxl345::readCalibratedData()
{
	Axes rawAxes = readRawData().axes;

	AdxlData data;
	data.axes = calculateCalbiratedAxes(rawAxes);
	data.rotate = calculateRotate(data.axes);

	return data;
}

void Adxl345::updateData()
{
	data = readData();
}

AdxlData Adxl345::getData()
{
	return data;
}
