#include <OOCSI.h>

// SSID of your Wifi network, the library currently does not support WPA2 Enterprise networks
const char* ssid = "Cirera";  //Ziggo5744712
// Password of your Wifi network.
const char* password = "marcirera";   //SQAEEJQB1a!

// name for connecting with OOCSI (unique handle)
const char* OOCSIName = "Test2";
// put the adress of your OOCSI server here, can be URL or IP address string
const char* hostserver = "oocsi.id.tue.nl";

OOCSI oocsi = OOCSI();

//-------------------- Potentiometre COLOR-------------------------------//
int potPin = 33; // potentiometre
int potVal;
int potAverage;
int potCorrected;
int newAverage;
int oldAverage;
int divider = 25; // varage over 25 measurements
int oldVal;


//-------------------- Potentiometre BRIGNESS-------------------------------//
int BpotPin = 32; // potentiometre
int BpotVal;
int BpotAverage;
int BpotCorrected;
int BnewAverage;
int BoldAverage;
int Bdivider = 25; // varage over 25 measurements

// ---------------Emotion state-----------------//
int emotionalState;
int i = 0;
int x = 0;

//---------------LED STRIP----------------------//
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 14 // simple led
#define LED_PIN_STRIP 12 // led strip
#define LED_COUNT 1
#define STRIP_COUNT 58

int brightness;
uint32_t color;
uint32_t colorled;
uint32_t colorstrip;


Adafruit_NeoPixel led(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip(STRIP_COUNT, LED_PIN_STRIP, NEO_GRBW + NEO_KHZ800);

//-------------------------button-----------------//
unsigned long previousTime = 0;
unsigned long currentTime = 0;
//button and led
const int buttonPin = 34;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
//int buttonPin;

// variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


void setup() {
  Serial.begin(115200);
  // WIFI AND CONECCTION
  // -------------------WIFI-------------//


  //WiFi.begin(ssid, password);
  //output OOCSI activity on the built - in LED
  //pinMode(LED_BUILTIN, OUTPUT);
  //oocsi.setActivityLEDPin(LED_BUILTIN);
  //oocsi.connect(OOCSIName, hostserver, ssid, password);

  //------------------POTENTIOMETRES------------//
  pinMode(potPin, INPUT);
  pinMode(BpotPin, INPUT);
  //--------------LED and STRIP ---------//
  strip.begin();
  strip.show();
  led.begin();
  led.show();

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

}

void loop() {
  //oocsi.newMessage("Helloworld");
  //oocsi.addString("device_id", "d0fdc0c31d2284d6f");
  // ------------------------- Corrects potentiometre color --------------------------
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
  // ------------------------- Corrects potentiometre BRIGTHNESS--------------------------
  for (int j = 0; j < Bdivider; j++) {
    BpotVal = analogRead(BpotPin);
    BpotVal = BpotVal >> 2;
    BnewAverage = BoldAverage + BpotVal;
    BoldAverage = BnewAverage;
  }
  BpotCorrected = BnewAverage / Bdivider;
  Serial.print("BPotentiometer = ");
  Serial.print(BpotCorrected);
  Serial.print("\t");
  BnewAverage = 0;
  BoldAverage = 0;
  //--------------------------------Average potentiometer Value---------------------------------------
  Serial.print("oldVal = ");
  Serial.print(oldVal);
  Serial.print("\t");
  Serial.print("EmotionalState = ");
  Serial.print(emotionalState);
  Serial.print("\t");
  Serial.print("i = ");
  Serial.print(i);
  Serial.print("\t");
  Serial.print("x = ");
  Serial.println(x);
  //----------------LED BRUGHTNESS --------//
  if (BpotCorrected > 0 && BpotCorrected < 341) {//LOW
    brightness = 25;
  }
  if (BpotCorrected > 342 && BpotCorrected < 682) {//MID
    brightness = 100;
  }
  if (BpotCorrected > 683 ) {//HIG
    brightness = 250;
  }
  //----------------LED COLOR--------//
  if (potCorrected > 0 && potCorrected < 140) { //Fear - Purple
    emotionalState = 1;
    color = led.Color(128, 0, 128, 0);
    led.setPixelColor(0, color);
    //brightness = 50;
    led.setBrightness(brightness);
    led.show();

  }
  if (potCorrected > 140 && potCorrected < 290) { //Anger - Red
    emotionalState = 2;
    color = led.Color(255, 0, 0, 0);
    led.setPixelColor(0, color);
    //brightness = 50;
    led.setBrightness(brightness);
    led.show();

  }
  if (potCorrected > 290 && potCorrected < 474) { //Suprise - Darkorange
    emotionalState = 3;
    color = led.Color(255, 140, 0, 0);
    led.setPixelColor(0, color);
    //brightness = 50;
    led.setBrightness(brightness);
    led.show();

  }
  if (potCorrected > 474 && potCorrected < 640) { //Happy - Yellow
    emotionalState = 4;
    color = led.Color(255, 255, 0, 0);
    led.setPixelColor(0, color);
    //brightness = 50;
    led.setBrightness(brightness);
    led.show();

  }
  if ((potCorrected > 640 && potCorrected < 795)) { //Disgust - Green
    emotionalState = 5;
    color = led.Color(0, 255, 0, 0);
    led.setPixelColor(0, color);
    //brightness = 50;
    led.setBrightness(brightness);
    led.show();

  }
  if (potCorrected > 795 && potCorrected < 960) { //Sad - Blue
    emotionalState = 6;
    color = led.Color(0, 0, 255, 0);
    led.setPixelColor(0, color);
    //brightness = 50;
    led.setBrightness(brightness);
    led.show();

  }
  if (potCorrected > 960) { //Neutral White
    emotionalState = 7;
    color = led.Color(255, 255, 255, 0);
    led.setPixelColor(0, color);
    //brightness = 50;
    led.setBrightness(brightness);
    led.show();

  }
  // --------------------------------------------- button -----------------
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;


      if (buttonState == HIGH) {
        strip.setPixelColor(i, color);
        strip.setBrightness(brightness);
        strip.show();
        x = x + 1;

        //oocsi.addInt("EmotionalState", emotionalState);
        //oocsi.sendMessage();
        //oocsi.check();
      }
      i = x;
    }
  }
  lastButtonState = reading;

}
