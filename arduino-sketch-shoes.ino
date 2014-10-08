#include <Pinoccio.h>
#include <Scout.h>

#include <SPI.h>
#include <Wire.h>
#include <Scout.h>
#include <GS.h>
#include <bitlash.h>
#include <lwm.h>
#include <js0n.h>
#include <Adafruit_NeoPixel.h>
#include <NeoPixelStripAnimator.h>

#define PIN 6


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//
// NOTE: The settings in here have been tested on a set of 6 "Flora RGB Smart NeoPixel version 2"
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.


// This is the instance of the NeoPixel strip animator
NeoPixelStripAnimator neoPixelStripAnimator(&strip);

String red = "50", green = "50", blue = "50";

numvar neopixelClear(void)
{
  neoPixelStripAnimator.clear();
}

numvar startCustomWipeAnim(void)
{
  neoPixelStripAnimator.startAnimation(new ColorWipeAnimation(Adafruit_NeoPixel::Color(getarg(1), getarg(2), getarg(3)), 1000));
}

numvar neopixelChangeColor(void){
  numvar new_red = getarg(1);
  numvar new_green = getarg(2);
  numvar new_blue = getarg(3);

  red = new_red;
  green = new_green;
  blue = new_blue;
}

void setup() {
  Scout.setup();
  neoPixelStripAnimator.setup();

  addBitlashFunction("pixels.clear", (bitlash_function)neopixelClear);
  addBitlashFunction("pixels.change", (bitlash_function)neopixelChangeColor);
  addBitlashFunction("pixels.flash", (bitlash_function)startCustomWipeAnim);
}

bool step = false;

void loop() {
  if(!neoPixelStripAnimator.isAnimationRunnning()){
    delay(3000);
    neoPixelStripAnimator.startAnimation(new FadeOutAnimation(Adafruit_NeoPixel::Color(red.toInt(),green.toInt(),blue.toInt()), 1000));
  }
  Scout.loop();
  neoPixelStripAnimator.loop();
}
