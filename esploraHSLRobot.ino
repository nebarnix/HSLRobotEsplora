// include the Esplora library
#include <Esplora.h>
#include <SoftwareSerial.h>
#define JOY_DEADZONE 5

bool S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, S13, S14, S15, S16;
char r1 = 8;
char r2 = 0;
char r3 = 1;
//char r4 = ;

char c1 = 7;
char c2 = 16;
char c3 = 15;
char c4 = 14;

unsigned long time = 0;

char soundBank = 0;

SoftwareSerial mySerial(3, 11); // RX, TX

void scanButtonMatrix()
{
  digitalWrite(r1, LOW);
  digitalWrite(r2, HIGH);
  digitalWrite(r3, HIGH);

  S1 = !digitalRead(c1);
  S2 = !digitalRead(c2);
  S3 = !digitalRead(c3);
  S4 = !digitalRead(c4);

  digitalWrite(r1, HIGH);
  digitalWrite(r2, LOW);
  digitalWrite(r3, HIGH);

  S5 = !digitalRead(c1);
  S6 = !digitalRead(c2);
  S7 = !digitalRead(c3);
  S8 = !digitalRead(c4);

  digitalWrite(r1, HIGH);
  digitalWrite(r2, HIGH);
  digitalWrite(r3, LOW);

  S9 = !digitalRead(c1);
  S10 = !digitalRead(c2);
  S11 = !digitalRead(c3);
  S12 = !digitalRead(c4);

  /*
    digitalWrite(r1,LOW);
    digitalWrite(r2,LOW);
    digitalWrite(r3,LOW);
    digitalWrite(r4,HIGH);

    S13 = digitalRead(c1);
    S14 = digitalRead(c2);
    S15 = digitalRead(c3);
    S16 = digitalRead(c4);*/
}
/*
  inline unsigned int readTinkerkitInputA() {
    return readChannel(CH_TINKERKIT_A);
  }
  inline unsigned int readTinkerkitInputB() {
    return readChannel(CH_TINKERKIT_B);
  }*/

bool readTinkerkitInputAButton()
{
  unsigned int val = Esplora.readTinkerkitInputA();
  return (val > 512) ? HIGH : LOW;
}

bool readTinkerkitInputBButton()
{
  unsigned int val = Esplora.readTinkerkitInputB();
  char count;
  return (val > 512) ? HIGH : LOW;
}


void setup() {
  // nothing to setup
  //Serial1.begin(2400); //hardware
  Serial.begin(9600); //USB, stuck at 9600...
  mySerial.begin(2400); //software
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);

  pinMode(c1, INPUT_PULLUP);
  pinMode(c2, INPUT_PULLUP);
  pinMode(c3, INPUT_PULLUP);
  pinMode(c4, INPUT_PULLUP);
}

void loop() {
  // read the sensor into a variable
  int slider = 1023 - Esplora.readSlider();

  // convert the sensor readings to light levels
  int throttle  = slider / 4;


  scanButtonMatrix();

  int soundID = decodeSoundButtons();

  if (soundID > 0)
  {
    mySerial.print("SND=");
    mySerial.print(soundID);
    mySerial.print(';');

    //Serial.print("SND=");
    //Serial.print(soundID);
    //Serial.println(';');
  }
  else if (Esplora.readButton(SWITCH_UP) == LOW) //LOW means pressed
  {
    //Esplora.writeBlue(0);
    //Esplora.writeRed(throttle / 4);
    //mySerial.print("FWD=");
    //mySerial.print(throttle, DEC);
    //mySerial.print(';');

    //Serial.print("FWD=");
    //Serial.print(throttle, DEC);
    //Serial.print(';');

    mySerial.print("RAW=");
    mySerial.print(throttle, DEC);
    mySerial.print(',');
    mySerial.print(map(-Esplora.readJoystickX(),-512,512,int(-throttle*0.9),int(throttle*0.9))); //limit side to side motion for controllability in mario kart mode
    mySerial.print(';');
  }
  else if (Esplora.readButton(SWITCH_DOWN) == LOW) //LOW means pressed
  {
    //Esplora.writeRed(0);
    //Esplora.writeBlue(throttle / 4);
    mySerial.print("REV=");
    mySerial.print(throttle, DEC);
    mySerial.print(';');

    //Serial.print("REV=");
    //Serial.print(throttle, DEC);
    //Serial.print(';');
  }
  else if (Esplora.readButton(SWITCH_LEFT) == LOW) //LOW means pressed
  {
    //Esplora.writeRed(0);
    //Esplora.writeBlue(0);
    //Esplora.writeGreen(throttle / 4);
    mySerial.print("LFT=");
    mySerial.print(int(constrain(throttle * 1.5, 0, 255)), DEC);
    mySerial.print(';');

    //Serial.print("LFT=");
    //Serial.print(int(constrain(throttle*1.5,0,255)), DEC);
    //Serial.print(';');
  }
  else if (Esplora.readButton(SWITCH_RIGHT) == LOW) //LOW means pressed
  {
    //Esplora.writeRed(0);
    //Esplora.writeBlue(0);
    //Esplora.writeGreen(throttle / 4);
    mySerial.print("RGT=");
    mySerial.print(int(constrain(throttle * 1.5, 0, 255)), DEC);
    mySerial.print(';');

    //Serial.print("RGT=");
    //Serial.print(int(constrain(throttle*1.5,0,255)), DEC);
    //Serial.print(';');
  }
  else if (Esplora.readJoystickSwitch() == LOW) //LOW means pressed
  {
    mySerial.print("BEEP;");
  }
  else
  {
    // No buttons pressed. Go ahead and read the joystick.
    //throttle; //The range of our sensor is +512 to -512. We need the slider range to match to simplify our mapping (-slider to + slider).
    long int joyX = -Esplora.readJoystickX();
    int joyY = -Esplora.readJoystickY();
    //define a dead zone in the very middle of the stick to stop from sending lots of 0 commands
    if (joyX > JOY_DEADZONE || joyX < -JOY_DEADZONE || joyY > JOY_DEADZONE || joyY < -JOY_DEADZONE)
    {

      //limit the top speed based on the slider
      joyY = map(joyY, -512, 512, -throttle, throttle); //map from -512,512 to -speed +speed, so max speed is from (-255 to +255)

      joyX = map(joyX, -512, 512, -throttle * 1.5, throttle * 1.5); //turns seem sluggish, maybe speeding up the X axis will help
      joyX = constrain(joyX, -255, 255); //clip to limit, else the following squaring will mess up if throttle*1.5 is above 255

      //map turn rate to the square
      if (joyX > 0)
        joyX = (joyX * joyX) / (255);
      else
        joyX = -((joyX * joyX) / (255));

      mySerial.print("RAW=");
      mySerial.print(constrain(joyY, -255, 255), DEC);
      mySerial.print(',');
      mySerial.print(constrain(joyX, -255, 255), DEC);
      mySerial.print(';');

      //Serial.print("RAW=");
      //Serial.print(constrain(joyY, -255, 255), DEC);
      //Serial.print(',');
      //Serial.print(constrain(joyX, -255, 255), DEC);
      //Serial.print(';');
      //Serial.print(Esplora.readJoystickY());
      //Serial.print('\t');
      //Serial.println(Esplora.readJoystickX());
    }
  }
  // add a small delay to keep the LED from flickering:
  delay(150); //update rate is 10hz

  //Serial.print(millis()-time);
  //time = millis();
}

int decodeSoundButtons()
{
  //light up light banks
  if (S1 == HIGH)
  {
    Esplora.writeRed(32);
    Esplora.writeGreen(0);
    Esplora.writeBlue(0);
    soundBank = 0;
  }
  else if (S2 == HIGH)
  {
    Esplora.writeRed(0);
    Esplora.writeGreen(32);
    Esplora.writeBlue(0);
    soundBank = 8;
  }
  else if (S3 == HIGH)
  {
    Esplora.writeBlue(32);
    Esplora.writeRed(0);
    Esplora.writeGreen(0);
    soundBank = 16;
  }
  else if (S4 == HIGH)
  {
    if (soundBank == 24)
    {
      soundBank = 32;
    Esplora.writeBlue(32);
    Esplora.writeRed(32);
    Esplora.writeGreen(32);
    }
    else
    {
      Esplora.writeBlue(0);
      Esplora.writeRed(0);
      Esplora.writeGreen(0);
      soundBank = 24;
    }

  }

  if (S5 == HIGH)
    return soundBank + 1;

  if (S6 == HIGH)
    return soundBank + 2;

  if (S7 == HIGH)
    return soundBank + 3;

  if (S8 == HIGH)
    return soundBank + 4;

  if (S9 == HIGH)
    return soundBank + 5;

  if (S10 == HIGH)
    return soundBank + 6;

  if (S11 == HIGH)
    return soundBank + 7;

  if (S12 == HIGH)
    return soundBank + 8;

  return -1;
}
