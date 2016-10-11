

// include the library code:
#include <LiquidCrystal.h>

const int buttonPin = 8;     // the number of the pushbutton pin
const int ledPin =  9;      // the number of the LED pin


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int buttonState = 0;         // variable for reading the pushbutton status
int status = 1;

//FAST LED STUFF BELOW HERE

#include <FastLED.h>

#define LED_PIN  6

#define COLOR_ORDER GRB
#define CHIPSET     WS2811

#define BRIGHTNESS 64

// Params for width and height
const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 8;

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = false;

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }
  
  return i;
}

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* leds( leds_plus_safety_pixel + 1);

uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if( x >= kMatrixWidth) return -1;
  if( y >= kMatrixHeight) return -1;
  return XY(x,y);
}

void setup() {

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("Funday Friday!!!");

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);


}

void loop() {


  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  //digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(200);              // wait for a second

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:


  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, LOW);



    if (status == 1) {
      
      lcd.begin(16, 2);
      lcd.print("Funday Friday!!!");
      status = 0;

    } else {

  // Now turn the LED off, then pause
  FastLED.clear();
  FastLED.show();
  delay(500);

    }


  } else {
    // turn LED off:
    digitalWrite(ledPin, HIGH);
    lcd.print(" ");
    status = 1;

    //FAST LED STUFF BELOW HERE
    uint32_t ms = millis();
    int32_t yHueDelta32 = ((int32_t)cos16( ms * (27/1) ) * (350 / kMatrixWidth));
    int32_t xHueDelta32 = ((int32_t)cos16( ms * (39/1) ) * (310 / kMatrixHeight));
    DrawOneFrame( ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
    if( ms < 5000 ) {
      FastLED.setBrightness( scale8( BRIGHTNESS, (ms * 256) / 5000));
    } else {
      FastLED.setBrightness(BRIGHTNESS);
    }
    FastLED.show();
  }

}

void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  byte lineStartHue = startHue8;
  for( byte y = 0; y < kMatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;      
    for( byte x = 0; x < kMatrixWidth; x++) {
      pixelHue += xHueDelta8;
      leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
    }
  }
}


