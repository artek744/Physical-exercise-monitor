#include <Adxl345.h>
#include <math.h>

#define MAX_NUMBER_OF_DEVICES 7

enum PostureMeasurement {INDEFINITE, CORRECT, INCORRECT};
enum IncorrectPartBodyPoture {NOTHING, ARMS, BACK, ALL};

// ----------------------------------------------------------
// -------------------- DEVICES_ADDRESS ---------------------
// ----------------------------------------------------------

#define TCAADDR 0x70


// ----------------------------------------------------------
// -------------------- CALIBRATION DATA --------------------
// ----------------------------------------------------------

#define DEV1_OFFSET_X -63.50
#define DEV1_OFFSET_Y -66.50
#define DEV1_OFFSET_Z 172.00
#define DEV1_GAIN_X 131.50
#define DEV1_GAIN_Y 131.00
#define DEV1_GAIN_Z 138.00

#define DEV4_OFFSET_X -0.50
#define DEV4_OFFSET_Y -11.00
#define DEV4_OFFSET_Z -5.50
#define DEV4_GAIN_X 127.50
#define DEV4_GAIN_Y 128.00
#define DEV4_GAIN_Z 123.50

#define DEV2_OFFSET_X -1.50
#define DEV2_OFFSET_Y -1.50
#define DEV2_OFFSET_Z 3.50
#define DEV2_GAIN_X 130.50
#define DEV2_GAIN_Y 131.50
#define DEV2_GAIN_Z 128.00

#define DEV3_OFFSET_X 12.00
#define DEV3_OFFSET_Y -13.50
#define DEV3_OFFSET_Z -18.00
#define DEV3_GAIN_X 132.00
#define DEV3_GAIN_Y 132.00
#define DEV3_GAIN_Z 127.50


// ----------------------------------------------------------
// ------------------- SETTING PARAMETERS -------------------
// ----------------------------------------------------------

//Boki
#define ROLL_THRESHOLD_FOR_ARMS_POSTURE 20
//Góra dół
#define PITCH_THRESHOLD_FOR_ARMS_POSTURE 12

//Boki
#define ROLL_THRESHOLD_FOR_BACK_POSTURE 15
//Góra dół
#define PITCH_THRESHOLD_FOR_BACK_POSTURE 30

#define SOUND_FRAME_TIME_MSEC 50
#define SOUND_FRAME_NUMBER 32


// ----------------------------------------------------------
// -------------------- GLOBAL VARIABLES --------------------
// ----------------------------------------------------------

Adxl345 adxlDev1 = Adxl345();
Adxl345 adxlDev2 = Adxl345();
Adxl345 adxlDev3 = Adxl345();
Adxl345 adxlDev4 = Adxl345();

int ledPin = 8;

bool wrongArmsPostureSong[SOUND_FRAME_NUMBER] = {1,0, 1,0, 1,0, 1,0, 1,1, 0,0, 0,0, 0,0, 1,0, 1,0, 1,0, 1,0, 1,1, 0,0, 0,0, 0,0};
bool wrongBackAndArmsPostureSound[SOUND_FRAME_NUMBER] = {1,1, 1,1, 1,1, 1,1, 1,1, 1,1, 0,0, 0,0, 1,1, 1,1, 1,1, 1,1, 1,1, 1,1, 0,0, 0,0};
bool wrongBackPostureSound[SOUND_FRAME_NUMBER] = {1,1, 0,0, 0,0, 1,1, 0,0, 0,0, 1,1, 0,0, 0,0, 1,1, 0,0, 0,0, 1,1, 0,0, 0,0, 0,0};

PostureMeasurement pm = INDEFINITE;
IncorrectPartBodyPoture ipbp = NOTHING;

static int soundFrameCounter = -1;
static long int lastTime = millis();
bool sound = LOW;


void tcaselect(uint8_t deviceNumber) {
  if (deviceNumber > MAX_NUMBER_OF_DEVICES) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << deviceNumber);
  Wire.endTransmission();
}

void setupDevice(const Adxl345 *device, Axes offset, Axes gain) {
  device->powerOn();
  device->setRangeSetting(4);            
  device->calibrate(offset, gain);
}

Axes createCalibrationStructure(float x, float y, float z)
{
  Axes calibrationStructure;
  calibrationStructure.x = x;
  calibrationStructure.y = y;
  calibrationStructure.z = z;

  return calibrationStructure;
}

void showData(AdxlData data, int devNumber) {
  Serial.print("DEV:"); Serial.println(devNumber); 
  Serial.print("PITCH: "); Serial.print(data.rotate.pitch); Serial.print("    ROLL: "); Serial.println(data.rotate.roll);
  Serial.print("X:"); Serial.print(data.axes.x); Serial.print("  Y:"); Serial.print(data.axes.y); Serial.print("  Z:"); Serial.print(data.axes.z); 
  Serial.println();
}

bool isCorrect(Rotate rotate1, Rotate rotate2, int rollThreshold, int pitchThreshold, int secondRollMultiplier = 1, int secondPitchMultiplier = 1)
{
  if(abs(rotate1.roll - rotate2.roll*secondRollMultiplier) > rollThreshold) {
    Serial.print("roll: "); Serial.println(abs(rotate1.roll - rotate2.roll*secondRollMultiplier));
    return false ;
  }

  if(abs(rotate1.pitch - rotate2.pitch*secondPitchMultiplier) > pitchThreshold) {
    Serial.print("pitch: "); Serial.println(abs(rotate1.pitch - rotate2.pitch*secondPitchMultiplier));
    return false ;
  }

  return true;
}

void signaling(bool sound_array[], PostureMeasurement *pm_flag)
{

  
  if(*pm_flag != INCORRECT) {
    return;
  }

  if(millis() - lastTime > SOUND_FRAME_TIME_MSEC) {
    lastTime = millis();
    soundFrameCounter++;
    
    if(soundFrameCounter >= SOUND_FRAME_NUMBER) {
      soundFrameCounter = -1;
      *pm_flag = INDEFINITE;
      digitalWrite(ledPin, LOW);
      ipbp = NOTHING;
      return;
    }

    sound = sound_array[soundFrameCounter];
  }

  digitalWrite(ledPin, sound);
}


void setup()
{
  pinMode(ledPin, OUTPUT);
  
  Wire.begin();
  Serial.begin(9600);  // start serial for output
  Serial.println("\n\nSTART:\n");

  Axes offsetDev1 = createCalibrationStructure(DEV1_OFFSET_X, DEV1_OFFSET_Y, DEV1_OFFSET_Z);
  Axes gainDev1 = createCalibrationStructure(DEV1_GAIN_X, DEV1_GAIN_Y, DEV1_GAIN_Z);
  
  Axes offsetDev2 = createCalibrationStructure(DEV2_OFFSET_X, DEV2_OFFSET_Y, DEV2_OFFSET_Z);
  Axes gainDev2 = createCalibrationStructure(DEV2_GAIN_X, DEV2_GAIN_Y, DEV2_GAIN_Z);
  
  Axes offsetDev3 = createCalibrationStructure(DEV3_OFFSET_X, DEV3_OFFSET_Y, DEV3_OFFSET_Z);
  Axes gainDev3 = createCalibrationStructure(DEV3_GAIN_X, DEV3_GAIN_Y, DEV3_GAIN_Z);
  
  Axes offsetDev4 = createCalibrationStructure(DEV4_OFFSET_X, DEV4_OFFSET_Y, DEV4_OFFSET_Z);
  Axes gainDev4 = createCalibrationStructure(DEV4_GAIN_X, DEV4_GAIN_Y, DEV4_GAIN_Z);
  
  tcaselect(0);
  setupDevice(&adxlDev1, offsetDev1, gainDev1);
  tcaselect(1);
  setupDevice(&adxlDev2, offsetDev2, gainDev2);
  tcaselect(2);
  setupDevice(&adxlDev3, offsetDev3, gainDev3);
  tcaselect(3);
  setupDevice(&adxlDev4, offsetDev4, gainDev4);
}

void loop()
{
  tcaselect(0);
  adxlDev1.updateData();
  tcaselect(1);
  adxlDev2.updateData();
  tcaselect(2);
  adxlDev3.updateData();
  tcaselect(3);
  adxlDev4.updateData();

  showData(adxlDev2.getData(), 2);
  showData(adxlDev3.getData(), 3);
  Serial.println();
  Serial.println();
  
  if(isCorrect(adxlDev2.getData().rotate, adxlDev3.getData().rotate, ROLL_THRESHOLD_FOR_ARMS_POSTURE, PITCH_THRESHOLD_FOR_ARMS_POSTURE, -1)) {
    if(pm == INDEFINITE) {
      pm = CORRECT;
    }
  }
  else {
    pm = INCORRECT;
    ipbp = ARMS;
  }

// MONITORING BACK POSTURE
  if(isCorrect(adxlDev1.getData().rotate, adxlDev4.getData().rotate, ROLL_THRESHOLD_FOR_BACK_POSTURE, PITCH_THRESHOLD_FOR_BACK_POSTURE, -1, -1)) {
    if(pm == INDEFINITE) {
      pm = CORRECT;
    }
  }
  else {
    pm = INCORRECT;
    if(ipbp == ARMS) {
      ipbp = ALL;
    }
    else {
      ipbp = BACK;
    }
  }
  
  switch(ipbp) {
    case ARMS: 
      signaling(wrongArmsPostureSong, &pm);
      break;
    case BACK:
      signaling(wrongBackPostureSound, &pm);
      break;
    case ALL:
      signaling(wrongBackAndArmsPostureSound, &pm);
      break;    
  }

//  delay(2000);



}





