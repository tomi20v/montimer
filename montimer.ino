// https://forum.arduino.cc/index.php?topic=590442

// normal run, without jumpers
#define NORMAL_BOOTTIME 25000000
#define NORMAL_RUNTIME 180000000
#define NORMAL_THROTTLE 80
#define NORMAL_AFTERRUN_THROTTLE 60
// test run, both trainer pin (0) and full pin (2) closed to ground with jumper
#define TEST_BOOTTIME 6000000
#define TEST_RUNTIME 5000000
#define TEST_THROTTLE NORMAL_THROTTLE
#define TEST_AFTERRUN_THROTTLE NORMAL_AFTERRUN_THROTTLE
// trainer pin (0) closed to ground
#define TRAINER_BOOTTIME NORMAL_BOOTTIME
#define TRAINER_RUNTIME 60000000
#define TRAINER_THROTTLE 70
#define TRAINER_AFTERRUN_THROTTLE NORMAL_AFTERRUN_THROTTLE
// full pin (2) closed to ground
#define FULL_BOOTTIME NORMAL_BOOTTIME
#define FULL_THROTTLE 90
#define FULL_RUNTIME NORMAL_RUNTIME
#define FULL_AFTERRUN_THROTTLE NORMAL_THROTTLE
#define AFTER_RUNTIME 5000000

// connect esc signal pin to pin 4. An easy and nice servo connection can be made on a trinket board following the "Touch and Go" tutorial linked earlier.
byte throttlePin = 4;
byte trainerPin = 0;
byte testPin = 1;
byte fullPin = 2;
unsigned long sTime;
unsigned long cTime;
unsigned long elapsed;
unsigned int val;
unsigned int delayMs; // 

unsigned long bootTime = NORMAL_BOOTTIME;
unsigned long runTime = NORMAL_RUNTIME;
unsigned long afterRunTime = AFTER_RUNTIME;
unsigned int throttleNormal = NORMAL_THROTTLE;
unsigned int throttleAfter = NORMAL_AFTERRUN_THROTTLE;

void setup()
{

  pinMode(throttlePin, OUTPUT);
  digitalWrite(throttlePin, LOW);

  pinMode(trainerPin, INPUT_PULLUP);
  pinMode(testPin, INPUT_PULLUP);
  pinMode(fullPin, INPUT_PULLUP);

  // test run: connect both trainerPin and fullPin to GND
  // only 6 second initial delay (enough for ESC to initialize), then normal throttle for 10 seconds, then afterRun for 10 seconds
  if ((digitalRead(trainerPin) == LOW) && (digitalRead(fullPin) == LOW)) {
    bootTime = TEST_BOOTTIME;
    runTime = TEST_RUNTIME;
    throttleNormal = TEST_THROTTLE;
    throttleAfter = TEST_AFTERRUN_THROTTLE;
  }
  // trainer run: connect only trainerPin to GND
  // 1min runtime, low throttle
  else if (digitalRead(trainerPin) == LOW) {
    bootTime = TRAINER_BOOTTIME;
    throttleNormal = TRAINER_THROTTLE;
    runTime = TRAINER_RUNTIME;
  }
  // full throttle run: connect only fullPin to GND
  // normal runtime, more throttle
  else if (digitalRead(fullPin) == LOW) {
    bootTime = FULL_BOOTTIME;
    throttleNormal = FULL_THROTTLE;
    runTime = FULL_RUNTIME;
  }

  //turn off L13
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);


} //setup

// take throttle pcnt and send HIGH pulse for the calculated period that correspondents to that percentage
void throttlePcnt(unsigned int pcnt) {
  // to try
  delayMs = 1050 + pcnt*9; // 1050 .. 1950 => 1050 + pcnt * 900 / 100
//  delayMs = 1100 + pcnt*8; // 1100 .. 1900 => 1100 + pcnt * 800 / 100
  digitalWrite(throttlePin, HIGH);
  delayMicroseconds(delayMs);
  digitalWrite(throttlePin, LOW);
}

// each loop is trimmed so it takes 2000ms to execute, thus each loop is a servo signal cycle
// note: this code does not consider time taken to run the loop, so servo cycle is actually a bit longer (but works fine with ESCs)
void loop()
{

    // pulse cycle start
    sTime = micros();
    // all off
    if (sTime > bootTime + runTime + afterRunTime) {
      throttlePcnt(0);
      digitalWrite(LED_BUILTIN, LOW);
    }
    // afterrun
    else if (sTime > bootTime + runTime) {
      throttlePcnt(throttleAfter);
      if (sTime % 500000 < 250000) {
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
    // normal run
    else if (sTime > bootTime) {
      throttlePcnt(throttleNormal);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    // boot delay / wait, flash led every 0.5 seconds (twice per sec)
    else {
      throttlePcnt(0);
      if (sTime % 500000 < 250000) {
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }

//    if (millis() > 30*60*1000) {
    if (millis() > 180000) {
      digitalWrite(throttlePin, LOW);
      while (1>0) {
        if (micros() % 300000 < 100000) {
          digitalWrite(LED_BUILTIN, HIGH);
        }
        else {
          digitalWrite(LED_BUILTIN, LOW);
        }
      }
    }

//    // redundant here, we already switched it off in throttlePcnt()
//    digitalWrite(throttlePin, LOW);
    delayMs = sTime - micros() + 20000; // 20000 - elapsed Ms in this loop => 20000 - (micros() - sTime)

//    // original "poor mans servo" code example
//    // I used this code as normal servo libraries seemed to work with servos but not with ESCs.
//    // Also this way I don't need interrupts etc.
//    digitalWrite(throttlePin, HIGH);
//    delayMicroseconds(1900);    //full
//    //delayMicroseconds(1100);    //min
//    digitalWrite(throttlePin, LOW);
//    delayMicroseconds(18100);   //full balance of 20000 cycle
//    //delayMicroseconds(18900);   //full balance of 20000 cycle

} //loop
