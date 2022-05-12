#include <OOCSI.h>

#include <FastLED.h>
#define LED_PIN    25
#define NUM_LEDS   21
#define BRIGHTNESS 60
#define VOLTS 5
#define MAX_AMPS 500
CRGB leds[NUM_LEDS];

#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#define UTC_OFFSET_IN_SECONDS 7200       // offset from greenwich time

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", UTC_OFFSET_IN_SECONDS);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// SSID of your Wifi network, the library currently does not support WPA2 Enterprise networks
const char* ssid = "Ziggo5744712";  //Ziggo5744712
// Password of your Wifi network.
const char* password = "SQAEEJQB1a!";   //SQAEEJQB1a!

// name for connecting with OOCSI (unique handle)
const char* OOCSIName = "Tracker";
// put the adress of your OOCSI server here, can be URL or IP address string
const char* hostserver = "oocsi.id.tue.nl";

int potPin = 33;
int potVal;
int potAverage;
int potCorrected;
int newAverage;
int oldAverage;
int divider = 25;
int actualTime;
int timeFrame;
int ledState = -1;
String ledColor = "White";
boolean userInput;
int oldVal;
int emotionalState;
OOCSI oocsi = OOCSI();



void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(potPin, INPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  timeClient.begin();
  timeClient.update();

  int hourVal = (timeClient.getHours());
  if (hourVal > 3 && hourVal < 12) {
    timeFrame = 0;
  }
  if (hourVal > 12 && hourVal < 18) {
    timeFrame = 1;
  }
  if (hourVal > 18) {
    timeFrame = 2;
  }

  potVal = analogRead(potPin);
  potVal = potVal >> 2;
  oldVal = potVal;
  emotionalState = 0;

  // output OOCSI activity on the built-in LED
  pinMode(LED_BUILTIN, OUTPUT);
  oocsi.setActivityLEDPin(LED_BUILTIN);
  oocsi.connect(OOCSIName, hostserver, ssid, password);
}

void loop() {
  oocsi.newMessage("MaxAlberts1996");
  oocsi.addString("device_id", "d096ee00545e54de2");

  timeClient.update();
  //  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  //  Serial.print(", ");

  int secondVal = (timeClient.getSeconds());
  Serial.print("secondVal = ");
  Serial.print(secondVal);
  Serial.print("\t");

  int hourVal = (timeClient.getHours());
  Serial.print("hourVal = ");
  Serial.print(hourVal);
  Serial.print("\t");

  Serial.print("TimeFrame = ");
  Serial.print(timeFrame);
  Serial.print("\t");

  Serial.print("Actual Time = ");
  Serial.print(actualTime);
  Serial.print("\t");

  if (hourVal > 3 && hourVal < 12) {
    actualTime = 1;
  }
  if (hourVal > 12 && hourVal < 18) {
    actualTime = 2;
  }
  if (hourVal > 18) {
    actualTime = 3;

  }


  if (actualTime == 1 && timeFrame == 0) {
    oocsi.addInt("Evening emotion", emotionalState);
    oocsi.sendMessage();
    oocsi.check();
    ledState = ledState + 1;
    leds[ledState] = CRGB::White;
    emotionalState = 0;
    FastLED.show();
    timeFrame = timeFrame + 1;

  }
  if (actualTime == 2 && timeFrame == 1) {
    oocsi.addInt("Morning Emotion", emotionalState);
    oocsi.sendMessage();
    oocsi.check();
    ledState = ledState + 1;
    leds[ledState] = CRGB::White;
    emotionalState = 0;
    FastLED.show();
    timeFrame = timeFrame + 1;
  }
  if (actualTime == 3 && timeFrame == 2) {
    oocsi.addInt("Afternoon Emotion", emotionalState);
    oocsi.sendMessage();
    oocsi.check();
    ledState = ledState + 1;
    leds[ledState] = CRGB::White;
    emotionalState = 0;
    FastLED.show();
    timeFrame = 0;
  }


  //---------------------------------Seconds example--------------------------------------------------


  //  if (secondVal > 0 && secondVal < 20) {
  //    actualTime = 1;
  //  }
  //  if (secondVal > 20 && secondVal < 40) {
  //    actualTime = 2;
  //  }
  //  if (secondVal > 40 && secondVal < 60) {
  //    actualTime = 3;
  //  }
  //
  //  if (actualTime == 1 && timeFrame == 0) {
  //    oocsi.addInt("Evening emotion", emotionalState);
  //    oocsi.sendMessage();
  //    oocsi.check();
  //    ledState = ledState + 1;
  //    leds[ledState] = CRGB::White;
  //    emotionalState = 0;
  //    FastLED.show();
  //    timeFrame = timeFrame + 1;
  //
  //  }
  //  if (actualTime == 2 && timeFrame == 1) {
  //    oocsi.addInt("Morning Emotion", emotionalState);
  //    oocsi.sendMessage();
  //    oocsi.check();
  //    ledState = ledState + 1;
  //    leds[ledState] = CRGB::White;
  //    emotionalState = 0;
  //    FastLED.show();
  //    timeFrame = timeFrame + 1;
  //  }
  //  if (actualTime == 3 && timeFrame == 2) {
  //    oocsi.addInt("Afternoon Emotion", emotionalState);
  //    oocsi.sendMessage();
  //    oocsi.check();
  //    ledState = ledState + 1;
  //    leds[ledState] = CRGB::White;
  //    emotionalState = 0;
  //    FastLED.show();
  //    timeFrame = 0;
  //  }
  //---------------------------------Seconds example--------------------------------------------------

  //--------------------------------Average potentiometer Value---------------------------------------

  for (int j = 0; j < divider; j++) {
    potVal = analogRead(potPin);
    potVal = potVal >> 2;
    newAverage = oldAverage + potVal;
    oldAverage = newAverage;
  }
  potCorrected = newAverage / divider;
  Serial.print("Potentiometer = ");
  Serial.print(potCorrected);
  Serial.print("\t");
  newAverage = 0;
  oldAverage = 0;

  //--------------------------------Average potentiometer Value---------------------------------------

  if ((potCorrected - oldVal) > 5 || (potCorrected - oldVal) < -5) {
    userInput = true;
  }

  Serial.print("oldVal = ");
  Serial.print(oldVal);
  Serial.print("userInput = ");
  Serial.print(userInput);
  Serial.print("\t");
  Serial.print("EmotionalState = ");
  Serial.println(emotionalState);

  if (potCorrected == 0) {
    leds[ledState] = CRGB::White;
  }

  if (userInput == true) {

    if (potCorrected == 0) {
      leds[ledState] = CRGB::White;
    }

    if (potCorrected > 0 && potCorrected < 140) { //Fear
      emotionalState = 1;
      leds[ledState] = CRGB::Purple;
    }
    if (potCorrected > 140 && potCorrected < 290) { //Anger
      emotionalState = 2;
      leds[ledState] = CRGB::Red;
    }
    if (potCorrected > 290 && potCorrected < 474) { //Suprise
      emotionalState = 3;
      leds[ledState] = CRGB::DarkOrange;
    }
    if (potCorrected > 474 && potCorrected < 640) { //Happy
      emotionalState = 4;
      leds[ledState] = CRGB::Yellow;
    }
    if ((potCorrected > 640 && potCorrected < 795)) { //Disgust
      emotionalState = 5;
      leds[ledState] = CRGB::Green;
    }
    if (potCorrected > 795 && potCorrected < 960) { //Sad
      emotionalState = 6;
      leds[ledState] = CRGB::Blue;
    }
    if (potCorrected > 960) { //Neutral
      emotionalState = 7;
      leds[ledState] = CRGB::White;
    }

    FastLED.show();
    oldVal = potCorrected;
    userInput = false;
  }
}
