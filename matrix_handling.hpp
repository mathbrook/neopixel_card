#include <FastLED.h>
#include "boardconfig.hpp"
#include <FastLED_NeoMatrix.h>
#include <Adafruit_GFX.h>
//tried to define matrix stuff in a separate file, but this didnt work
//so i put the matrix stuff in the .ino itself
#ifndef _MATRIX_HANDLER_H
#define _MATRIX_HANDLER_H
extern CRGB leds[NUM_LEDS];
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, 3, 3, 3, 3, 
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
    NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE + 
    NEO_TILE_TOP + NEO_TILE_LEFT +NEO_TILE_COLUMNS);

const uint16_t colors[] = {
  matrix->Color(255, 0, 0), matrix->Color(0, 255, 0), matrix->Color(0, 0, 255) };
int matrix_x    = mw;
int matrix_pass = 0;
void matrix_setup(){
  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(40);
  matrix->setTextColor(colors[0]);
}
void matrix_loop(){
  matrix->fillScreen(0);
  matrix->setCursor(matrix_x, 0);
  matrix->print(F("Yous"));
  if(--matrix_x < -36) {
    matrix_x = matrix->width();
    if(++matrix_pass >= 3) matrix_pass = 0;
    matrix->setTextColor(colors[matrix_pass]);
  }
  matrix->show();
  delay(100);
}
#endif
