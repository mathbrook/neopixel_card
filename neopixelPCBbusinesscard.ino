/// @file    Blink.ino
/// @brief   Blink the first LED of an LED strip
/// @example Blink.ino

#include <FastLED.h>
#include <stdio.h>
#include "twinklefox.hpp"
#include "noise_party.hpp"
#include "boardconfig.hpp"
CRGBArray<NUM_LEDS> leds_twinklefox;
//array to translate led positions
const uint8_t actual_led_number[81] ={0,1,2,27,28,29,54,55,56,
                                      3,4,5,30,31,32,57,58,59,
                                      6,7,8,33,34,35,60,61,62,
                                      9,10,11,36,37,38,63,64,65,
                                      12,13,14,39,40,41,66,67,68,
                                      15,16,17,42,43,44,69,70,71,
                                      18,19,20,45,46,47,72,73,74,
                                      21,22,23,48,49,50,75,76,77,
                                      24,25,26,51,52,53,78,79,80};



const uint8_t change_mode_time = 30;
unsigned long current_time = 0;
unsigned long last_time = 0;
uint8_t light_pattern_state = 1; //set the first pattern to start with
// Define the array of leds
CRGB leds[NUM_LEDS];

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }
#define BRIGHTNESS 64
//colortemp defines:
#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky

// How many seconds to show each temperature before switching
#define DISPLAYTIME 5
// How many seconds to show black between switches
#define BLACKTIME   1


void setup() { 
  delay(100);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  digitalWrite(A0,LOW);
 digitalWrite(A1,LOW);
 digitalWrite(A2,LOW);
 digitalWrite(A3,LOW);
 digitalWrite(A4,LOW);
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  FastLED.setBrightness( BRIGHTNESS );
  // Initialize our coordinates to some random values
  noise_setup();
  chooseNextColorPalette(gTargetPalette);
  light_pattern_state = random(0,4);
}

void loop() { 
  current_time = millis();
  if(current_time-last_time > (change_mode_time * 1000)){
    last_time = millis();
    light_pattern_state++;
    //update when adding new patterns
    if(light_pattern_state > 4){
      light_pattern_state=0;
    }
  }
  // int a = digitalRead(A0);
  // int b = digitalRead(A1);
  // int c = digitalRead(A2);
  // int d = digitalRead(A3);
  // int e = digitalRead(A4);
  // char buf[200];
  // sprintf(buf,"Joystick A: %d, B: %d, C: %d, D: %d, E: %d\n",a,b,c,d,e);
  // Serial.print(buf);
  // delay(500);
  state_machine(light_pattern_state);
}

void cylon(){
  static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[actual_led_number[i]] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
  Serial.print("x");

  // Now go in the other direction.  
  for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    // Set the i'th led to red 
    leds[actual_led_number[i]] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
}

void update_tictactoe_block(int block,int color){
  int r = (color << 16);
  int g = (color <<8);
  int b = (color);
  for(int i=0;i<9;i++){
    leds[i+(block*9)]=CRGB(r,g,b);
  }
  FastLED.show();
}

void colortemperature(){
  // draw a generic, no-name rainbow
  static uint8_t starthue = 0;
  fill_rainbow( leds + 5, NUM_LEDS - 5, --starthue, 20);

  // Choose which 'color temperature' profile to enable.
  uint8_t secs = (millis() / 1000) % (DISPLAYTIME * 2);
  if( secs < DISPLAYTIME) {
    FastLED.setTemperature( TEMPERATURE_1 ); // first temperature
    leds[0] = TEMPERATURE_1; // show indicator pixel
  } else {
    FastLED.setTemperature( TEMPERATURE_2 ); // second temperature
    leds[0] = TEMPERATURE_2; // show indicator pixel
  }

  // Black out the LEDs for a few secnds between color changes
  // to let the eyes and brains adjust
  if( (secs % DISPLAYTIME) < BLACKTIME) {
    memset8( leds, 0, NUM_LEDS * sizeof(CRGB));
  }
  
  FastLED.show();
  FastLED.delay(8);
}
// uint16_t XY( uint8_t x, uint8_t y)
// {
//   uint16_t i;

//   if( kMatrixSerpentineLayout == false) {
//     i = (y * kMatrixWidth) + x;
//   }

//   if( kMatrixSerpentineLayout == true) {
//     if( y & 0x01) {
//       // Odd rows run backwards
//       uint8_t reverseX = (kMatrixWidth - 1) - x;
//       i = (y * kMatrixWidth) + reverseX;
//     } else {
//       // Even rows run forwards
//       i = (y * kMatrixWidth) + x;
//     }
//   }

//   return i;
// }
// // Fill the x/y array of 8-bit noise values using the inoise8 function.
// void fillnoise8() {
//   for(int i = 0; i < MAX_DIMENSION; i++) {
//     int ioffset = scale * i;
//     for(int j = 0; j < MAX_DIMENSION; j++) {
//       int joffset = scale * j;
//       noise[i][j] = inoise8(x + ioffset,y + joffset,z);
//     }
//   }
//   z += speed;
// }


// void noise_pattern(){
//   static uint8_t ihue=0;
//   fillnoise8();
//   for(int i = 0; i < kMatrixWidth; i++) {
//     for(int j = 0; j < kMatrixHeight; j++) {
//       // We use the value at the (i,j) coordinate in the noise
//       // array for our brightness, and the flipped value from (j,i)
//       // for our pixel's hue.
//       leds[actual_led_number[XY(i,j)]] = CHSV(noise[j][i],255,noise[i][j]);

//       // You can also explore other ways to constrain the hue used, like below
//       // leds[XY(i,j)] = CHSV(ihue + (noise[j][i]>>2),255,noise[i][j]);
//     }
//   }
//   ihue+=1;

//   FastLED.show();
//   // delay(10);
// }

void state_machine(int state){
  switch(state){
    case 0:
    {
      cylon();
      break;
    }
    case 1:
    {
      twinklefox_function();
      break;
    }
    case 2:
    {
      noise_loop();
      break;
    }
    case 3:
    {
      pride();
      FastLED.show();
      break;
    }
    case 4:
    {
      EVERY_N_MILLISECONDS( 20) {
        pacifica_loop();
        FastLED.show();
      }
    }
  }
}

void pride() 
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
    nblend( leds[actual_led_number[pixelnumber]], newcolor, 64);
  }
}

#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120
#define FRAMES_PER_SECOND 60
bool gReverseDirection = false;

void Fire2012()
{
// Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[actual_led_number[pixelnumber]] = color;
    }
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };


void pacifica_loop()
{
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));

  // Clear out the LED array to a dim background blue-green
  fill_solid( leds, NUM_LEDS, CRGB( 2, 6, 10));

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer( pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
  pacifica_one_layer( pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  pacifica_one_layer( pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer( pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps();

  // Deepen the blues and greens a bit
  pacifica_deepen_colors();
}

// Add one layer of waves into the led array
void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps()
{
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void pacifica_deepen_colors()
{
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8( leds[i].blue,  145); 
    leds[i].green= scale8( leds[i].green, 200); 
    leds[i] |= CRGB( 2, 5, 7);
  }
}