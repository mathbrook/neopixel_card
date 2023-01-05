#include <FastLED.h>
#ifndef _BOARDCONFIG_H
#define _BOARDCONFIG_H
// How many leds in your strip?
#define NUM_LEDS 81
//for some fucking reason the data pin is 2, i dont understand arduino shit
#define DATA_PIN PD2
//#define CLOCK_PIN 13
#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)
// #define NUM_LEDS (kMatrixWidth * kMatrixHeight)

#define mw 9
#define mh 9
#define NUMMATRIX (mw*mh)

#define FRAMES_PER_SECOND  120

#endif
