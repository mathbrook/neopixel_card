/// @file    Blink.ino
/// @brief   Blink the first LED of an LED strip
/// @example Blink.ino

#include <FastLED.h>
#include <stdio.h>
// How many leds in your strip?
#define NUM_LEDS 81
#define PIXELS_FOR_SOC 80
//for some fucking reason the data pin is 2, i dont understand arduino shit
#define DATA_PIN PD2
//#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];
void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical

}

void loop() { 
  state_machine(0);
}

void cylon(){
  static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
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
    leds[i] = CHSV(hue++, 255, 255);
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

void state_machine(int state){
  switch(state){
    case 0:
    {
      cylon();
      break;
    }
  }
}
