// Libraries
#include <Adafruit_NeoPixel.h>

byte DATAPIN = 52; // Arduino PWM data pin D6
byte NUMPIXELS = 66;   // Number of neopixels
const byte NUMROWS = 7;
const byte NUMCOLS = 10;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, DATAPIN, NEO_GRB + NEO_KHZ800);

const byte glassesMatrix[NUMROWS][NUMCOLS] = {
  {37, 36, 35, 34, 33,   32, 31, 30, 29, 28},
  {38, 39, 40, 41, 42,   23, 24, 25, 26, 27},
  {47, 46, 45, 44, 43,   22, 21, 20, 19, 18},
  {48, 49, 50, 51, 52,   13, 14, 15, 16, 17},
  {57, 56, 55, 54, 53,   12, 11, 10, 9, 8},
  {58, 59, 60, 61, 62,   3,   4,  5, 6, 7},
      {65, 64, 63,          2, 1, 0}
};

void setup() {
  pixels.begin();
  pixels.clear();
  pixels.show();
}

void loop() {
  for (int i=0; i<NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, 50));
    pixels.show();
    delay(50);
    if(i%3==0){
      pixels.clear(); 
    }  
  }
//  for (int i=NUMPIXELS; i>=0; i--){
//    pixels.setPixelColor(i, 0); 
//    pixels.show(); 
//    delay(1);
//  }
}
