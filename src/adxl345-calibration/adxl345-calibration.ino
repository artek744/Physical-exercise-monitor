#include <Adxl345.h>

Adxl345 adxl = Adxl345();

int counter=0;

int AccelMinX = 1000;
int AccelMaxX = -1000;
int AccelMinY = 1000;
int AccelMaxY = -1000;
int AccelMinZ = 1000;
int AccelMaxZ = -1000; 

void setup()
{
  Serial.begin(9600);
  Serial.println("START");
  adxl.powerOn();
  adxl.setRangeSetting(4);

  Serial.print("Attempt: ");
}

void loop()
{

  Serial.print(counter+1);
  Serial.print("  ");
  
  while (!Serial.available()){}
  
  Axes raw;
  adxl.updateData();
  raw = adxl.getData().axes;

  if(raw.x < AccelMinX) AccelMinX = raw.x;
  if(raw.x > AccelMaxX) AccelMaxX = raw.x;

  if(raw.y < AccelMinY) AccelMinY = raw.y;
  if(raw.y > AccelMaxY) AccelMaxY = raw.y;

  if(raw.z < AccelMinZ) AccelMinZ = raw.z;
  if(raw.z > AccelMaxZ) AccelMaxZ = raw.z;

  while (Serial.available())
  {
    Serial.read();
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
    Serial.print("GAIN: "); Serial.print(gainX); Serial.print(" ");Serial.print(gainY); Serial.print(" "); Serial.print(gainZ); Serial.println();
    Serial.println();
    while(1);
  }
}




