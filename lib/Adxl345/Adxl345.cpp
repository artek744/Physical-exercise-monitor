#include "Adxl345.h"

Adxl345::Adxl345(int deviceAddr)
{
   deviceAddress=deviceAddr;
}

void Adxl345::powerOn()
{
   Wire.begin();
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

void Adxl345::calibrate(const AccelCalibration &offset, const AccelCalibration &gain)
{
   this->offsetX = offset.x;
   this->offsetY = offset.y;
   this->offsetZ = offset.z;
   this->gainX = gain.x;
   this->gainY = gain.y;
   this->gainZ = gain.z;
}

void Adxl345::writeTo(byte regAddress, byte val) 
{
   Wire.beginTransmission(deviceAddress); //start transmission to device 
   Wire.write(regAddress);        // send register address
   Wire.write(val);        // send value to write
   Wire.endTransmission(); //end transmission
}

void Adxl345::readFrom(byte regAddress, int num, byte buff[]) 
{
   Wire.beginTransmission(deviceAddress); //start transmission to device 
   Wire.write(regAddress);        //sends address to read from
   Wire.endTransmission(); //end transmission
   
   Wire.beginTransmission(deviceAddress); //start transmission to device (initiate again)
   Wire.requestFrom(deviceAddress, num);    // request 6 bytes from device
   
   int i = 0;
   while(Wire.available())    //device may send less than requested (abnormal)
   { 
     buff[i] = Wire.read(); // receive a byte
     i++;
   }
   Wire.endTransmission(); //end transmission
}

AccelRaw Adxl345::readAccelRaw() 
{
   byte _buff[6];
   readFrom(ADXL345_DATAX0, ADXL345_TO_READ, _buff); //read the acceleration data from the ADXL345
 
   AccelRaw raw;
   raw.x = (((int)_buff[1]) << 8) | _buff[0];
   raw.y = (((int)_buff[3]) << 8) | _buff[2];
   raw.z = (((int)_buff[5]) << 8) | _buff[4];
   
   return raw;
}

AccelCalibration Adxl345::readAccelCalibrated()
{
   AccelRaw raw;
   raw = readAccelRaw();

   AccelCalibration calibrated; 

   calibrated.x = (raw.x - offsetX)/gainX;
   calibrated.y = (raw.y - offsetY)/gainY;
   calibrated.z = (raw.z - offsetZ)/gainZ;
   
   return calibrated;   
}
