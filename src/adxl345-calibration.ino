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

void setup()
{
  Serial.begin(9600);  // start serial for output
  
  adxl.powerOn();
  adxl.setRangeSetting(4);
  Serial.print("Attempt: ");            
}

void loop()
{
  
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
    Serial.print("OFFSET:  x:"); Serial.print(offX); Serial.print("   y:");Serial.print(offY); Serial.print("   z:"); Serial.print(offZ); Serial.println();
    Serial.print("GAIN:  x:"); Serial.print(gainX); Serial.print("   y:");Serial.print(gainY); Serial.print("   z:"); Serial.print(gainZ); Serial.println();
    Serial.println();
    ;     
  }
}





