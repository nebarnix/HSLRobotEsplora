// include the Esplora library
#include <Esplora.h>
#define JOY_DEADZONE 5

void setup() {
  // nothing to setup
  Serial1.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // read the sensor into a variable
  int slider = 1023 - Esplora.readSlider();

  // convert the sensor readings to light levels
  byte bright  = slider / 4;


  if (Esplora.readButton(SWITCH_UP) == LOW) //LOW means pressed
  {
    //Esplora.writeBlue(0);
    Esplora.writeRed(bright);
    Serial1.print("FWD=");
    Serial1.print(bright, DEC);
    Serial1.print(';');
  }
  else if (Esplora.readButton(SWITCH_DOWN) == LOW) //LOW means pressed
  {
    //Esplora.writeRed(0);
    Esplora.writeBlue(bright);
    Serial1.print("REV=");
    Serial1.print(bright, DEC);
    Serial1.print(';');
  }
  else if (Esplora.readButton(SWITCH_LEFT) == LOW) //LOW means pressed
  {
    //Esplora.writeRed(0);
    //Esplora.writeBlue(0);
    Esplora.writeGreen(bright);
    Serial1.print("LFT=");
    Serial1.print(constrain(bright*1.5,0,255), DEC);
    Serial1.print(';');
  }
  else if (Esplora.readButton(SWITCH_RIGHT) == LOW) //LOW means pressed
  {
    //Esplora.writeRed(0);
    //Esplora.writeBlue(0);
    Esplora.writeGreen(bright);
    Serial1.print("RGT=");
    Serial1.print(constrain(bright*1.5,0,255), DEC);
    Serial1.print(';');
  }
  else
  {
    // No buttons pressed. Go ahead and read the joystick.
    //bright; //The range of our sensor is +512 to -512. We need the slider range to match to simplify our mapping (-slider to + slider). 
    int joyX = -Esplora.readJoystickX(); 
    int joyY = -Esplora.readJoystickY();
    if (joyX > JOY_DEADZONE || joyX < -JOY_DEADZONE || joyY > JOY_DEADZONE || joyY < -JOY_DEADZONE)
    {
      //limit the top speed based on the slider
      joyX = map(joyX,-512,512,-bright*1.5,bright*1.5); //turns seem sluggish, maybe speeding up the X axis will help
      joyY = map(joyY,-512,512,-bright,bright);
      Serial1.print("RAW=");
      Serial1.print(constrain(joyY,-255,255), DEC);
      Serial1.print(',');
      Serial1.print(constrain(joyX,-255,255), DEC);
      Serial1.print(';');
    }
  }
  // add a small delay to keep the LED from flickering:
  delay(100); //update rate is 10hz
}
