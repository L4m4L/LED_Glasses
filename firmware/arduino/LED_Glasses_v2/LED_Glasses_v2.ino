

// For the 8-bit art
//https://make8bitart.com/

// Libraries
#include <Adafruit_NeoPixel.h>    //
#include <Adafruit_NeoMatrix.h>   //
#include <Adafruit_GFX.h>         // https://learn.adafruit.com/adafruit-gfx-graphics-library/coordinate-system-and-units

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

// Declare the NeoMatrix
byte matrixWidth = 5;
byte matrixHeight = 7;
byte tilesX = 2;
byte tilesY = 1;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
  matrixWidth, matrixHeight, tilesX, tilesY, DATAPIN, NEO_TILE_BOTTOM+NEO_TILE_RIGHT+NEO_TILE_ROWS+NEO_TILE_ZIGZAG, NEO_GRB + NEO_KHZ800);

uint16_t colour = matrix.Color(0, 0, 0);
word frame = 0;

// Array definition (-1 = no LED)
const byte glassesMatrix[NUMROWS][NUMCOLS] = {
  {37, 36, 35, 34, 33,   32, 31, 30, 29, 28},
  {38, 39, 40, 41, 42,   23, 24, 25, 26, 27},
  {47, 46, 45, 44, 43,   22, 21, 20, 19, 18},
  {48, 49, 50, 51, 52,   13, 14, 15, 16, 17},
  {57, 56, 55, 54, 53,   12, 11, 10, 9,   8},
  {58, 59, 60, 61, 62,    3,  4,  5, 6,   7},
  {-1, 65, 64, 63, -1,   -1,  2,  1, 0,  -1}
};

void setup() {
  // NeoPixel setup
  pixels.begin();
  pixels.clear();
  pixels.setBrightness(60);
  pixels.show();

  // NeoMatrix setup
  matrix.begin();
  matrix.setRemapFunction(myRemapFn);
  matrix.setBrightness(25);
}

void loop() {
  frame++;
  if(frame>=65355){
    frame=0;  
  }
  //sineWave(frame, 1, 25);
  for(int i = 0; i<NUMROWS; i++){
    lightRow(i);
    delay(40);
  }
  pixels.clear();
  for(int i = 0; i<NUMCOLS; i++){
    lightCol(i);
    delay(40);
  }
  pixels.clear();
}

// Map the neo_matrix to my Matrix
uint16_t myRemapFn(uint16_t x, uint16_t y){
  return glassesMatrix[y][x];
}

// light rows when given the index (1 = bottom row, 6 = top row)
void lightRow(byte rowIndex){

  rowIndex = map(rowIndex, 1, 7, 6, 0);
  for (int j = 0; j < NUMCOLS; j++){
    pixels.setPixelColor(glassesMatrix[rowIndex][j], 1, 1, 1);
  }
  
  pixels.show();
}

void lightCol(byte colIndex){
  colIndex = map(colIndex, 1, 10, 9, 0);
  for (int j = 0; j < NUMROWS; j++){
    pixels.setPixelColor(glassesMatrix[j][colIndex], 1, 1, 1);
  }
  
  pixels.show();
}

// frame = global var to keep track of the frames
// AMPLITUDE = 0->1 for the amplitude of the signal
// FREQ = not really freq but range from 0-90
void sineWave(word &frame, float AMPLITUDE, byte FREQ){
  pixels.clear();
  
  float angle = 0.0;
  byte rowIndex=0;
  for(byte i=0; i<NUMCOLS; i++){
    
    // convert angle to radians
    angle= (((FREQ*i)%360)*71.0)/4068.0;
    double sine = sin(angle)*AMPLITUDE;

    // Peaks
    if(1.0>=sine && sine>=0.8){
      rowIndex = 5;
      // move along the fretboard
      pixels.setPixelColor(glassesMatrix[rowIndex][(i+frame)%NUMCOLS], pixels.Color(10, 10, 10));
      // static display
      //pixels.setPixelColor(fretArray[i][stringIndex], pixels.Color(255*BRIGHTNESS_FACTOR, 255*BRIGHTNESS_FACTOR, 255*BRIGHTNESS_FACTOR));
    }
    // Troughs
    else if(-0.8>=sine && sine>=-1.0){
      rowIndex = 0;
      // move along the fretboard
      pixels.setPixelColor(glassesMatrix[rowIndex][(i+frame)%NUMCOLS], pixels.Color(10, 10, 10));
      // static display
      //pixels.setPixelColor(fretArray[i][stringIndex], pixels.Color(255*BRIGHTNESS_FACTOR, 255*BRIGHTNESS_FACTOR, 255*BRIGHTNESS_FACTOR));
    }  
    // Middle(s)
    else{
      // top middle
      if(0.79>=sine && sine>=0.49){
        rowIndex = 4;
        // move along the glasses
        pixels.setPixelColor(glassesMatrix[rowIndex][(i+frame)%NUMCOLS], pixels.Color(10, 10, 10));
        // static display
        //pixels.setPixelColor(fretArray[i][stringIndex], pixels.Color(255*BRIGHTNESS_FACTOR, 255*BRIGHTNESS_FACTOR, 255*BRIGHTNESS_FACTOR));
      } 
      // middle top middle     
      if(0.60>=sine && sine>=-0.01){
        rowIndex = 3;
        // move along the glasses
        pixels.setPixelColor(glassesMatrix[rowIndex][(i+frame)%NUMCOLS], pixels.Color(10, 10, 10));
        // static display
        //pixels.setPixelColor(fretArray[i][stringIndex], pixels.Color(255*BRIGHTNESS_FACTOR, 255*BRIGHTNESS_FACTOR, 255*BRIGHTNESS_FACTOR));
      }  
      // middle lower middle    
      if(0.01>=sine && sine>=-0.60){
        rowIndex = 2;
        // move along the glasses
        pixels.setPixelColor(glassesMatrix[rowIndex][(i+frame)%NUMCOLS], pixels.Color(10, 10, 10));
        // static display
        //pixels.setPixelColor(fretArray[i][stringIndex], pixels.Color(255*BRIGHTNESS_FACTOR, 255*BRIGHTNESS_FACTOR, 255*BRIGHTNESS_FACTOR));
      }  
      // lower middle   
      if(-0.49>=sine && sine>=-0.79){
        rowIndex = 1;
        // move along the glasses
        pixels.setPixelColor(glassesMatrix[rowIndex][(i+frame)%NUMCOLS], pixels.Color(10, 10, 10));
        // static display
        //pixels.setPixelColor(fretArray[i][stringIndex], pixels.Color(255*BRIGHTNESS_FACTOR, 255*BRIGHTNESS_FACTOR, 255*BRIGHTNESS_FACTOR));
      }  
    }   
  } 
  pixels.show();
}
