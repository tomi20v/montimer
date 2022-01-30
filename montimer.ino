// https://forum.arduino.cc/index.php?topic=590442

byte servoPin = 4;
byte testPin = 0;
byte fullPin = 2;
unsigned long sTime;
unsigned long cTime;
//unsigned long bootTime = 10000000; // 10 sec
unsigned long bootTime = 15000000; // 15sec
//unsigned long runTime = 5000000; // 5sec
unsigned long runTime = 60000000; // 60sec
unsigned long afterRunTime = 10000000; // 10 sec
//unsigned long afterRunTime = 2000000; // 2 sec
unsigned long elapsed;
unsigned int val;

void setup()
{

  //turn off L13
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(servoPin, OUTPUT);
  digitalWrite(servoPin, LOW);

} //setup

void loop()
{
//  Serial.print("one side...");
//  while (digitalRead(buttonPin) == HIGH)
  while (1 > 0)
  {

    sTime = micros();
    // all off
    if (sTime > bootTime + runTime + afterRunTime) {
      digitalWrite(servoPin, HIGH);
      delayMicroseconds(1100);
      digitalWrite(LED_BUILTIN, LOW);
    }
    // afterrun
    else if (sTime > bootTime + runTime) {
      digitalWrite(servoPin, HIGH);
      delayMicroseconds(1400);
      if (sTime % 500000 < 250000) {
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
    // normal run
    else if (sTime > bootTime) {
      digitalWrite(servoPin, HIGH);
      delayMicroseconds(1800);    //full
      digitalWrite(LED_BUILTIN, HIGH);
    }
    // boot delay / wait
    else {
      digitalWrite(servoPin, HIGH);
      delayMicroseconds(1100);    //min
      if (sTime % 500000 < 250000) {
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }

    digitalWrite(servoPin, LOW);
    while (micros() - sTime < 20000) { //full balance of 20000 cycle
      delayMicroseconds(1);
    }

//    digitalWrite(servoPin, HIGH);
//    delayMicroseconds(1900);    //full
//    delayMicroseconds(1100);    //min
//    digitalWrite(servoPin, LOW);
//    delayMicroseconds(18100);   //full balance of 20000 cycle
//    delayMicroseconds(18900);   //full balance of 20000 cycle

  }


} //loop
