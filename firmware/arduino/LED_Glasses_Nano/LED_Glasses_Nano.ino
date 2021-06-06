// For the 8-bit art
//https://make8bitart.com/

// Libraries
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include <arduinoFFT.h>
#include <Fonts/Picopixel.h>

byte DATAPIN = 3; // Arduino PWM data pin D6
byte NUMPIXELS = 84;   // Number of neopixels

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, DATAPIN, NEO_GRB + NEO_KHZ800);

// Declare the NeoMatrix
byte matrixWidth = 7;
byte matrixHeight = 6;
byte tilesX = 2;
byte tilesY = 1;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
  matrixWidth, matrixHeight, tilesX, tilesY, DATAPIN, NEO_TILE_BOTTOM+NEO_TILE_RIGHT+NEO_TILE_ROWS+NEO_TILE_ZIGZAG, NEO_GRB + NEO_KHZ800);

// LED Array Setup
#define NUMROWS 6
#define NUMCOLS 14
#define NUMCOLOURS 15
byte BRIGHTNESS = 25;

// Millis setup
unsigned long previousMillis = 0;
byte interval = 10;

// push button setup
byte buttonPin = 2;
bool buttonState = false;
bool prevState = false;
byte numSel = 11;
byte sel = 0;
byte prevColour=0;
byte newColour=0;

// potentiometer setup
word potVal = 0;
float brightnessFactor = 1;

// microphone setup
const byte sampleWindow = 15;     // sample window width in ms (50ms = 20Hz)
unsigned int sample;
const byte maxScale = 6;
const byte redZone = 5;
const byte buffer_size = 14;
byte buffer_index = 0;
byte displayPeakVals [buffer_size];   // array to store previous 14 displayPeaks for display

// FFT Setup
#define SAMPLES 32    // must be a power of 2
#define SAMPLING_FREQ 1000  //Hz, must be less than 10000 due to ADC
double vReal[SAMPLES];
double vImag[SAMPLES];
char data_avgs[NUMCOLS];
arduinoFFT FFT = arduinoFFT();
unsigned long microseconds;
unsigned int sampling_period_us;

float red=0;    // values: 0.0, 0.25, 0.5 0.75, 1
float green=0;  
float blue=0;

#define NUMCOLOURS 15
uint16_t colour = matrix.Color(0, 0, 0);

float colourArray [NUMCOLOURS][3] = {
  {0, 0, 0},    // off            #0
  {1, 1, 1},    // white          #1
  {1, 0, 0},    // red            #2
  {0, 1, 0},    // green          #3
  {0, 0, 1},    // blue           #4
  {1, 1, 0},    // yellow         #5
  {1, 0, 1},    // purple         #6
  {0, 1, 1},    // torqouise      #7

  {0.5, 1, 1},  //light blue      #8
  {1, 0.5, 1},  //light pink      #9
  {1, 1, 0.5},  //light yellow    #10

  {0.5, 0, 1},  //grape           #11
  {1, 0, 0.5},  //dark pink       #12
  {0, 0.5, 1},  //pale blue       #13

  {0.75, 0.5, 0},//orange         #14
};

int fftArray[NUMCOLS] = {11, 11, 6, 6, 4, 4, 7, 7, 3, 3, 5, 5, 2, 2};

const byte glassesArray[NUMROWS][NUMCOLS] = {
  {48, 47, 46, 45, 44, 43, 42, 35, 36, 37, 38, 39, 40, 41},
  {49, 50, 51, 52, 53, 54, 55, 34, 33, 32, 31, 30, 29, 28},
  {62, 61, 60, 59, 58, 57, 56, 21, 22, 23, 24, 25, 26, 27},
  {63, 64, 65, 66, 67, 68, 69, 20, 19, 18, 17, 16, 15, 14},
  {76, 75, 74, 73, 72, 71, 70, 7, 8, 9, 10, 11, 12, 13},
  {77, 78, 79, 80, 81, 82, 83, 6, 5, 4, 3, 2, 1, 0}
};

// setup animations
word frame = 0;
byte centreX = 3;
byte centreY = 3;

// setup pong animation
byte x_coord = 0;
byte y_coord = 0;
bool x_dir = 0;
bool y_dir = 0;
byte x_upp_lim = 13;
byte x_low_lim = 0;
byte y_upp_lim = 5;
byte y_low_lim = 0;
byte red_pad_y = 2;
byte blue_pad_y = 2;
byte red_score = 0;
byte blue_score = 0;
bool scoreFlag = false;
byte prevFrame = 0;

// ---------------------------------------------------- SETUP ---------------------------------------------------- \\

void setup() {
  // put your setup code here, to run once:
  pixels.begin();
  pixels.setBrightness(BRIGHTNESS);
  pixels.show(); // Initialize all pixels to 'off'  

  matrix.begin();
  matrix.setRemapFunction(myRemapFn);
  matrix.setTextWrap(false);
  matrix.setBrightness(BRIGHTNESS);
  matrix.setFont(&Picopixel);

  // Push Button setup
  pinMode(buttonPin, INPUT_PULLUP);
  // potentiometer
  pinMode(A7, INPUT);

  // FFT Setup
//  ADCSRA = 0b11100101;      // set ADC to free running mode and set pre-scalar to 32 (0xe5)
//  ADMUX = 0b00000110;       // use pin A6 and external voltage reference
  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQ));

  Serial.begin(9600);

}

// ---------------------------------------------------- LOOP ---------------------------------------------------- \\

void loop() {
  //Serial.println(sel);
  //delay(1000);
  // frame check (reset once it hits 255)
  if(frame == 65535){
    frame = 0;
  }

  // Map pot value to brightness
  brightnessFactor = (float)map(analogRead(A7), 1008, 6, 0, 127);

  checkButton(frame);

  // Switch statement for the displays (Switch doesnt work???)
  if(sel==0){
    unsigned long currentMillis = millis();
    if((unsigned long) (currentMillis - previousMillis) >= 100){
      previousMillis = millis();
      pong(frame, x_coord, y_coord, x_dir, y_dir, red_pad_y, blue_pad_y, red_score, blue_score, scoreFlag, prevFrame);
//      Serial.print("Blue score: ");
//      Serial.println(blue_score);
//      Serial.print("Red score: ");
//      Serial.println(red_score);
    }
  }
  else if(sel==1){
    // red hearts (can change)
    setColour(2);
    unsigned long currentMillis = millis();
    if((unsigned long) (currentMillis - previousMillis) >= 100){
      previousMillis = millis();
      hearts(frame);
      frame++;
    }
  }
  else if(sel==2){
    setColour(1);
    unsigned long currentMillis = millis();
    if((unsigned long) (currentMillis - previousMillis) >= 20){
      previousMillis = millis();
      sideSide(frame);
      frame++;
    }
  }
  else if(sel==3){
    setColour(1);
    unsigned long currentMillis = millis();
    if((unsigned long) (currentMillis - previousMillis) >= 40){
      previousMillis = millis();
      upDown(frame);
      frame++;
    }
  } 
  else if(sel==4){
    unsigned long currentMillis = millis();
    if((unsigned long) (currentMillis - previousMillis) >= 0.1){
      previousMillis = millis();
      rainbow(frame);
      frame++;
    }
  } 
  else if(sel==5){
    unsigned long currentMillis = millis();
    if((unsigned long) (currentMillis - previousMillis) >= 1){  
      previousMillis = millis();
      colourWipeFrame(frame);
      frame++;
    }
  }
  else if(sel==6){
    unsigned long currentMillis = millis();
    if((unsigned long) (currentMillis - previousMillis) >= 50){
      previousMillis = millis();
      circles(frame);
      frame++;
    }
  }
  else if (sel==7){
    unsigned long currentMillis = millis();
    if((unsigned long) (currentMillis - previousMillis) >= 50){
      previousMillis = millis();
      fftFcn();
      matrix.show();
    }
  }
  else if (sel==8){
    // volume measurement scroll across screen
    volumeScroll();
  }
  else if (sel==9){
    // sine wave display
    unsigned long currentMillis = millis();
    if((unsigned long) (currentMillis - previousMillis) >= 30){
      previousMillis = millis();
      //sineDisplay(frame, amplitude, frequency
      sineDisplay(frame, 1.0, 30);
      frame++;
    }
  }
  else if (sel==10){
    // volume measurement scroll across screen
    volumeScroll();
  }
}

// ---------------------------------------------------- FUNCTIONS ---------------------------------------------------- \\
// ---------------------------------------------------- BACK-BONE ---------------------------------------------------- \\

// Button check
void checkButton(word &frame){
  // Push button
  buttonState = digitalRead(buttonPin);
  
  if(buttonState != prevState){
    if(buttonState == true){
        if(sel < numSel-1){
        sel++;
      }
      else {
        sel=0;
      }
    }
    frame=0;
    matrix.clear();
    pixels.clear();
    matrix.show();
    pixels.show();
    prevState=buttonState;
    delay(100);
    red_score = 0;
    blue_score = 0;
    Serial.println("Push");
  }
}

// Set colour
void setColour(byte sel){
  red = colourArray[sel][0];
  green = colourArray[sel][1];
  blue = colourArray[sel][2];
}

uint16_t myRemapFn(uint16_t x, uint16_t y){
  return glassesArray[y][x];
}

// ---------------------------------------------------- DISPLAY ---------------------------------------------------- \\
// sine wave
void sineDisplay(word &frame, float AMPLITUDE, byte FREQ){
  pixels.clear();
  
  float angle = 0.0;
  byte rowIndex=0;
  for(byte i=0; i<NUMCOLS; i++){
    
    // convert angle to radians
    angle= (((FREQ*i)%360)*71.0)/4068.0;
    double sine = sin(angle)*AMPLITUDE;

    // Peaks
    if(1.0>=sine && sine>=0.8){
      rowIndex = 0;
      // move along the fretboard
      pixels.setPixelColor(glassesArray[rowIndex][(i+frame)%NUMCOLS], pixels.Color(0, 255, 0));
      // static display
      //pixels.setPixelColor(glassesArray[rowIndex][i], pixels.Color(255, 255, 255));
    }
    // Troughs
    else if(-0.8>=sine && sine>=-1.0){
      rowIndex = 5;
      // move along the fretboard
      pixels.setPixelColor(glassesArray[rowIndex][(i+frame)%NUMCOLS], pixels.Color(0, 255, 0));
      // static display
      //pixels.setPixelColor(glassesArray[rowIndex][i], pixels.Color(255, 255, 255));
    }  
    // Middle(s)
    else{
      // top middle
      if(0.79>=sine && sine>=0.49){
        rowIndex = 1;
        // move along the fretboard
        pixels.setPixelColor(glassesArray[rowIndex][(i+frame)%NUMCOLS], pixels.Color(0, 255, 0));
        // static display
        //pixels.setPixelColor(glassesArray[rowIndex][i], pixels.Color(255, 255, 255));
      } 
      // middle top middle     
      if(0.60>=sine && sine>=-0.01){
        rowIndex = 2;
        // move along the fretboard
        pixels.setPixelColor(glassesArray[rowIndex][(i+frame)%NUMCOLS], pixels.Color(0, 255, 0));
        // static display
        //pixels.setPixelColor(glassesArray[rowIndex][i], pixels.Color(255, 255, 255));
      }  
      // middle lower middle    
      if(0.01>=sine && sine>=-0.60){
        rowIndex = 3;
        // move along the fretboard
        pixels.setPixelColor(glassesArray[rowIndex][(i+frame)%NUMCOLS], pixels.Color(0, 255, 0));
        // static display
        //pixels.setPixelColor(glassesArray[rowIndex][i], pixels.Color(255, 255, 255));
      }  
      // lower middle   
      if(-0.49>=sine && sine>=-0.79){
        rowIndex = 4;
        // move along the fretboard
        pixels.setPixelColor(glassesArray[rowIndex][(i+frame)%NUMCOLS], pixels.Color(0, 255, 0));
        // static display
        //pixels.setPixelColor(glassesArray[rowIndex][i], pixels.Color(255, 255, 255));
      }  
    }   
  } 
  pixels.show();
}

// x_coord, y_coord of ball, dir = direction (0 left or down, 1 right or up)
void pong(word &frame, byte &x_coord, byte &y_coord, bool &x_dir, bool &y_dir, byte &red_pad_y, byte &blue_pad_y, byte &red_score, byte &blue_score, bool &scoreFlag, byte &prevFrame){

  //matrix.clear();
  
  // set colour
  setColour(1);
  uint16_t colourBall = matrix.Color(red*brightnessFactor, green*brightnessFactor, blue*brightnessFactor); 
  setColour(2);
  uint16_t redPad = matrix.Color(red*brightnessFactor, green*brightnessFactor, blue*brightnessFactor);
  setColour(4);
  uint16_t bluePad = matrix.Color(red*brightnessFactor, green*brightnessFactor, blue*brightnessFactor);

  if (scoreFlag){
    matrix.clear();

    if(red_score >= 10 || blue_score >= 10){
//      if(red_score == 10){
//              
//      }
//      if(blue_score == 10){
//        
//      }
      
      red_score = 0;
      blue_score = 0;
    }
    
    else if (frame<prevFrame+20){
      matrix.setTextColor(bluePad);
      matrix.setCursor(9, 5);
      matrix.print(blue_score);

      matrix.setTextColor(redPad);
      matrix.setCursor(2, 5);
      matrix.print(red_score);
    }
    else{
      scoreFlag=false;
      prevFrame=0;
    }
  }

  // first call 
  else if (frame<=1){
     // initialise the ball in middle of glasses
     y_coord = random(1, 5);
     x_coord = random(6, 8);
     // flip direction
     x_dir != x_dir;
     y_dir != y_dir;

     // initialise the paddles
     blue_pad_y = 2;
     red_pad_y = 2;

     scoreFlag=true;
     prevFrame = frame;
  }

  else{

    // every subsequent call, move the ball
      // bounce off blue paddle
      if(x_coord == x_upp_lim){
        if((blue_pad_y-1<=y_coord) && (y_coord<=blue_pad_y+1))
           x_dir = x_dir^1;
        else{
           frame=0;
           red_score++;
           matrix.clear();
        }
      }
    
      // bounce off red paddle
      if(x_coord == x_low_lim){
        if((red_pad_y-1<=y_coord) && (y_coord<=red_pad_y+1)){
            x_dir = x_dir^1; 
          }
        else{
         frame=0;
         blue_score++;
         matrix.clear();  
        }
      }
      
      // bounce off top/bottom
      if(y_coord == y_low_lim || y_coord == y_upp_lim){
        y_dir = y_dir^1;
      }
      
      // left movement
      if(!x_dir){
        x_coord--;
      }
      // right movement
      else{
        x_coord++;
      }
      // down movement
      if(!y_dir){
        y_coord++;
      }
      // up movement
      else{
        y_coord--; 
      }
  
    byte blue_pad_rand = random(-1, 2);
    byte red_pad_rand = random(-1, 2);
  
    // paddle movement
    if(y_coord > 1 && y_coord < 4){
      blue_pad_y=y_coord+blue_pad_rand;
      red_pad_y=y_coord+red_pad_rand;    
    }

    matrix.clear();
  
    // draw paddles
    for (byte i=blue_pad_y-1; i <= blue_pad_y+1; i++){
      matrix.drawPixel(13, i, bluePad);
    }
  
    for (byte i=red_pad_y-1; i <= red_pad_y+1; i++){
      matrix.drawPixel(0, i, redPad);
    }
    
    // draw ball
    matrix.drawPixel(x_coord, y_coord, colourBall);
  }
  matrix.show();
  frame++;
}

void rainbow(word &frame){
  long firstPixelHue = frame*256;
  if(firstPixelHue>5*65536){
    firstPixelHue=0;
  }

  for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
        // Offset pixel hue by an amount to make one full revolution of the
        // color wheel (range of 65536) along the length of the strip
        // (strip.numPixels() steps):
        int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
        // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
        // optionally add saturation and value (brightness) (each 0 to 255).
        // Here we're using just the single-argument hue variant. The result
        // is passed through strip.gamma32() to provide 'truer' colors
        // before assigning to each pixel:
        pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
      }
   pixels.show();
}

void colourWipeFrame(word &frame){
  uint32_t colour;
  
  if(frame == 1){
    newColour=random(0,5);
    if(prevColour!=newColour){
      setColour(newColour); 
      prevColour=newColour; 
    }
    else{
      newColour=random(0,5);
      setColour(newColour);
      prevColour=newColour;
    }
    colour = pixels.Color(red*brightnessFactor, green*brightnessFactor, blue*brightnessFactor);
  }
  
  if(frame > NUMPIXELS){
    // reset animation
    frame = 0;
  }
  
  pixels.setPixelColor(frame-1, colour);
  pixels.show();

}

void fftFcn(){

//  // Green
//  setColour(3);
//  uint16_t colourLow = matrix.Color(red*brightnessFactor, green*brightnessFactor, blue*brightnessFactor);
//  // Red
//  setColour(2);
//  uint16_t colourHigh = matrix.Color(red*brightnessFactor, green*brightnessFactor, blue*brightnessFactor);
    
  // Sampling
  for(byte i=0; i<SAMPLES; i++){
    microseconds = micros();
    int value = analogRead(0);
    vReal[i]=value/8;
    vImag[i]=0;

    while(micros() < (microseconds + sampling_period_us)){
    }
  }

  // FFT
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  FFT.DCRemoval();
  //double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQ);

//  for(int i=0; i<(SAMPLES/2); i++)
//    {
//        /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
//
//        Serial.print((i * 1.0 * SAMPLING_FREQ) / SAMPLES, 1);
//        Serial.print(" ");
//        Serial.println(vReal[i], 1);    //View only this line in serial plotter to visualize the bins
//    }

  // process the values
  int vRealVals[NUMCOLS];
  int avgvReal = 0;
  for(byte i = 2; i<(SAMPLES/2); i++){
    vRealVals[i-2] = vReal[i];
    avgvReal+=vRealVals[i-2];
  }

  avgvReal = avgvReal/NUMCOLS;

  // Display results on Glasses
  for(byte i = 2; i<(SAMPLES/2); i++){
    int displayPeak = map(vReal[i], 0, 40+avgvReal, 0, maxScale);
    displayPeakVals[i-2] = displayPeak;
  }

  for(byte i=0; i<NUMCOLS; i++){
    
    setColour(fftArray[i]);
    uint16_t colour = matrix.Color(red*brightnessFactor, green*brightnessFactor, blue*brightnessFactor);
    
    for(byte j=0; j<=maxScale; j++){
      // clear pixels
      if(j >= displayPeakVals[i]){
        matrix.drawPixel(i, NUMROWS-j, 0);
      }
      // draw in green
      else if (j < redZone){
        matrix.drawPixel(i, NUMROWS-j, colour);
      }
      // red!
      else{
        matrix.drawPixel(i, NUMROWS-j, colour);
      }
    }
  }
 
  //delay(5000); 
}

void volumeScroll(){
  // colour GREEN
  setColour(3);
  uint16_t colourLow = matrix.Color(red*brightnessFactor, green*brightnessFactor, blue*brightnessFactor);
  // colour RED
  setColour(2);
  uint16_t colourHigh = matrix.Color(red*brightnessFactor, green*brightnessFactor, blue*brightnessFactor);

  // millis setup
  unsigned long currentMillis = millis();

  // mic setup
  unsigned int peakToPeak = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  //
  while( millis() - currentMillis < sampleWindow){
    sample = analogRead(0);
    // toss out spurious readings
    if (sample < 1024){
      if (sample > signalMax){
        signalMax = sample;  // save the max levels
      }
      else if (sample < signalMin){
        signalMin = sample;  // save just the min levels
      }
    }
  }

  // map 1v p-p level to max scale of display
  peakToPeak = signalMax - signalMin;
  int displayPeak = map(peakToPeak, 0, 255, 0, maxScale);

  // save the previous 14 display peaks
  displayPeakVals[buffer_index] = displayPeak;
  buffer_index++;
  if (buffer_index >= buffer_size){
    buffer_index=0;
  }

  for(byte k=0; k<NUMCOLS; k++){
    byte j = (buffer_index + k) % NUMCOLS;
    for (byte i=0; i<=maxScale; i++){

      // blank these pixels
      if(i >= displayPeakVals[j]){
        matrix.drawPixel(k, NUMROWS-i, 0);
      }
      // draw in green
      else if (i < redZone){
        matrix.drawPixel(k, NUMROWS-i, colourLow);
      }
      // red!
      else{
        matrix.drawPixel(k, NUMROWS-i, colourHigh);
      }
    }
  }
  
  matrix.show();

}

void circles(word &frame){

  uint16_t colour;
  
  if(frame == 1){
    setColour(random(2, NUMCOLOURS+1));  
    colour = matrix.Color(red*brightnessFactor, green*brightnessFactor, blue*brightnessFactor);
  }
  
  if(frame%5 == 0){
    // reset animation (circle has reached corner)
    frame = 0;
  }
  
  matrix.fillCircle(3, 3, frame, colour);
  matrix.fillCircle(3+7, 3, frame, colour);
  matrix.show();

  return frame;
}

void upDown (word &frame){
  pixels.clear();
  if (frame >= NUMROWS*2){
    frame = 0;
  }
  if (frame >= NUMROWS){
    lightRow((NUMROWS*2)-frame);
  }
  else{
    lightRow(frame+1);
  }
}

void sideSide (word &frame){
  pixels.clear();
  if (frame >= NUMCOLS*2){
    frame = 0;
  }
  if (frame >= NUMCOLS){
    lightCol((NUMCOLS*2)-frame);
  }
  else{
    lightCol(frame+1);
  }
}

void scanColsRows (word &frame){
  if (frame >=NUMROWS+NUMCOLS){
    frame = 0;
  }
  else if (frame < NUMCOLS){
    scanCols(frame);
  }
  else{
    word rowFrame = frame-NUMCOLS;
    scanRows(rowFrame);
  }
}

void scanRows(word &frame){
  if (frame >= NUMROWS){
    frame=0;
  }
  pixels.clear();
  lightRow(frame+1);
}

void scanCols(word &frame){
  if (frame >= NUMCOLS){
    frame=0;
  }
  pixels.clear();
  lightCol(frame+1);
}

// Heart shaped box
void hearts(word &frame){
  
  colour = matrix.Color(red*brightnessFactor, green*brightnessFactor, blue*brightnessFactor);
  // need this Serial.println????
  Serial.println(frame%2);

  if(frame%2==1){
    // left lens
    matrix.drawPixel(3, 4, colour);
    matrix.drawPixel(2, 3, colour);
    matrix.drawPixel(4, 3, colour);
    matrix.drawPixel(3, 5, colour);
    matrix.drawPixel(3, 2, colour);
    matrix.drawPixel(3, 3, colour);
    matrix.drawPixel(2, 2, colour);
    matrix.drawPixel(4, 1, colour);
    matrix.drawPixel(2, 1, colour);
    matrix.drawPixel(4, 2, colour);
    matrix.drawPixel(2, 4, colour);
    matrix.drawPixel(4, 4, colour);
    matrix.drawPixel(1, 0, colour);
    matrix.drawPixel(5, 0, colour);
    matrix.drawPixel(0, 1, colour);
    matrix.drawPixel(0, 2, colour);
    matrix.drawPixel(1, 1, colour);
    matrix.drawPixel(1, 2, colour);
    matrix.drawPixel(1, 3, colour);
    matrix.drawPixel(2, 0, colour);
    matrix.drawPixel(3, 1, colour);
    matrix.drawPixel(4, 0, colour);
    matrix.drawPixel(5, 1, colour);
    matrix.drawPixel(5, 2, colour);
    matrix.drawPixel(6, 1, colour);
    matrix.drawPixel(6, 2, colour);
    matrix.drawPixel(5, 3, colour);
  
    // right lens
    matrix.drawPixel(10, 4, colour);
    matrix.drawPixel(9, 3, colour);
    matrix.drawPixel(11, 3, colour);
    matrix.drawPixel(10, 5, colour);
    matrix.drawPixel(10, 2, colour);
    matrix.drawPixel(10, 3, colour);
    matrix.drawPixel(9, 2, colour);
    matrix.drawPixel(11, 1, colour);
    matrix.drawPixel(9, 1, colour);
    matrix.drawPixel(11, 2, colour);
    matrix.drawPixel(9, 4, colour);
    matrix.drawPixel(11, 4, colour);
    matrix.drawPixel(8, 0, colour);
    matrix.drawPixel(12, 0, colour);
    matrix.drawPixel(7, 1, colour);
    matrix.drawPixel(7, 2, colour);
    matrix.drawPixel(8, 1, colour);
    matrix.drawPixel(8, 2, colour);
    matrix.drawPixel(8, 3, colour);
    matrix.drawPixel(9, 0, colour);
    matrix.drawPixel(10, 1, colour);
    matrix.drawPixel(11, 0, colour);
    matrix.drawPixel(12, 1, colour);
    matrix.drawPixel(12, 2, colour);
    matrix.drawPixel(13, 1, colour);
    matrix.drawPixel(13, 2, colour);
    matrix.drawPixel(12, 3, colour);
    
    matrix.show();
  }
  else{
    matrix.clear();
    matrix.show();
  }
}

// ---------------------------------------------------- GENERIC LIGHT FCNS ---------------------------------------------------- \\

// light rows when given the index (1 = bottom row, 6 = top row)
void lightRow(byte rowIndex){

  rowIndex = map(rowIndex, 1, 6, 5, 0);
  for (int j = 0; j < NUMCOLS; j++){
    pixels.setPixelColor(glassesArray[rowIndex][j], red*brightnessFactor, green*brightnessFactor, blue*brightnessFactor);
  }
  
  pixels.show();
}

void lightCol(byte colIndex){
  colIndex = map(colIndex, 1, 14, 13, 0);
  for (int j = 0; j < NUMROWS; j++){
    pixels.setPixelColor(glassesArray[j][colIndex], red*brightnessFactor, green*brightnessFactor, blue*brightnessFactor);
  }
  
  pixels.show();
}

void lightAll(){

  for (byte i=0; i<=NUMPIXELS; i++){
    pixels.setPixelColor(i, red*brightnessFactor, green*brightnessFactor, blue*brightnessFactor);
  }
  pixels.show();
}

void randomDisplay(){
  for(byte i=0; i<=NUMPIXELS; i++){
    setColour(random(1,NUMCOLOURS+1));
    pixels.setPixelColor(i, red*brightnessFactor, green*brightnessFactor, blue*brightnessFactor);
  }
  pixels.show();  
}
