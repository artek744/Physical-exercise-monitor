#include <Adxl345.h>

Adxl345 adxl = Adxl345();

int counter=0;
// for range 8
//OFFSET:  x:-33.00   y:-34.50   z:85.00
//GAIN:  x:66.00   y:65.50   z:69.00


int AccelMinX = 1000;
int AccelMaxX = -1000;
int AccelMinY = 1000;
int AccelMaxY = -1000;
int AccelMinZ = 1000;
int AccelMaxZ = -1000; 


#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}


void setup()
{
  Serial.begin(9600);  // start serial for output
  Serial.println("START");
  adxl.powerOn();
  adxl.setRangeSetting(4);

  Serial.print("Attempt: ");
}

void loop()
{
  tcaselect(7);

  Serial.print(counter+1);
  Serial.print("  ");
  
  while (!Serial.available()){}
  
  AccelRaw raw;
  raw = adxl.readAccelRaw();

  if(raw.x < AccelMinX) AccelMinX = raw.x;
  if(raw.x > AccelMaxX) AccelMaxX = raw.x;

  if(raw.y < AccelMinY) AccelMinY = raw.y;
  if(raw.y > AccelMaxY) AccelMaxY = raw.y;

  if(raw.z < AccelMinZ) AccelMinZ = raw.z;
  if(raw.z > AccelMaxZ) AccelMaxZ = raw.z;
  
//  Serial.print("Accel Maximums: "); Serial.print(AccelMaxX); Serial.print("  ");Serial.print(AccelMaxY); Serial.print("  "); Serial.print(AccelMaxZ); Serial.println();
//  Serial.print("Accel Minimums: "); Serial.print(AccelMinX); Serial.print("  ");Serial.print(AccelMinY); Serial.print("  "); Serial.print(AccelMinZ); Serial.println();
//  Serial.println();
j
  while (Serial.available())
  {
    Serial.read();                    // Clear buffer
  }

  counter++;

  if(counter == 6) {
    double offX, offY, offZ;
    double gainX, gainY, gainZ;

    offX = 0.5 * (AccelMaxX + AccelMinX);
    offY = 0.5 * (AccelMaxY + AccelMinY);
    offZ = 0.5 * (AccelMaxZ + AccelMinZ);

    gainX = 0.5 * (AccelMaxX - AccelMinX);
    gainY = 0.5 * (AccelMaxY - AccelMinY);
    gainZ = 0.5 * (AccelMaxZ - AccelMinZ);

    Serial.println("CALIBRATED VALUES");
    Serial.print("OFFSET: "); Serial.print(offX); Serial.print(" ");Serial.print(offY); Serial.print(" "); Serial.print(offZ); Serial.println();
    Serial.print("GAIN: "); Serial.print(gainX); Serial.print(" ");Serial.print(gainY); Serial.print(" f
    "); Serial.print(gainZ); Serial.println();
    Serial.println();
    while(1);
  }
}




