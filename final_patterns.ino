#include <EEPROM.h>
#include "FastLED.h"
#define LED_TYPE    WS2811  //WS2811, WS2812, WS2812B
#define LED_PIN 3
#define NUM_LEDS 11
#define MAX_BRIGHTNESS 255
#define MIN_BRIGHTNESS 40
#define COLOR_ORDER GRB
#define BRIGHTNESS  200
#define FRAMES_PER_SECOND  120
CRGB leds[NUM_LEDS];
CRGB color_choice[22] = {CRGB::Blue,CRGB::BlueViolet,CRGB::Brown, CRGB::Aqua, CRGB::Chocolate,CRGB::Crimson,CRGB::DarkCyan,CRGB::DarkMagenta, CRGB::DarkGreen,CRGB::DarkOrchid,CRGB::DarkRed,CRGB::DarkTurquoise,CRGB::DarkViolet,CRGB::DeepPink,CRGB::DodgerBlue,CRGB::FireBrick, CRGB::Fuchsia,CRGB::Indigo,CRGB::Maroon,CRGB::MidnightBlue,CRGB::Navy,CRGB::OrangeRed};
int mode;
int hue = 60;
int divisor = 10;
uint8_t blendRate = 300;  // How fast to blend.  Higher is slower.  [milliseconds]

CHSV colorStart = CHSV(96,255,255);  // starting color
CHSV colorTarget = CHSV(192,255,255);  // target color
CHSV colorCurrent = colorStart;
CHSV side_colorStart = CHSV(96,255,255); 
CHSV side_colorTarget = CHSV(192,255,255); 
CHSV side_colorCurrent = side_colorStart;

void solid_red(){
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Red;
  }
 }


void solid_blue(){
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Blue;
  }
 }

 void solid_green(){
    for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Green;
  }
 }
 
void juggle() {
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void to_sides_target_color(){
   EVERY_N_MILLISECONDS(20){
    static uint8_t k;  // the amount to blend [0-255]
    if ( side_colorCurrent.h == side_colorTarget.h ) {  // Check if target has been reached
      side_colorStart = side_colorCurrent;
      side_colorTarget = CHSV(random8(),255,255);  // new target to transition toward
      k = 0;  // reset k value
    }

    side_colorCurrent = blend(side_colorStart, side_colorTarget, k, SHORTEST_HUES);
    leds[8] = side_colorCurrent;
    leds[9] = side_colorCurrent;
    leds[10] = side_colorCurrent;
    k++;
  }

  FastLED.show();
}

void to_target_color(){
   EVERY_N_MILLISECONDS(blendRate){
    static uint8_t k;  // the amount to blend [0-255]
    if ( colorCurrent.h == colorTarget.h ) {  // Check if target has been reached
      colorStart = colorCurrent;
      colorTarget = CHSV(random8(),255,255);  // new target to transition toward
      k = 0;  // reset k value
    }

    colorCurrent = blend(colorStart, colorTarget, k, SHORTEST_HUES);
    fill_solid( leds, NUM_LEDS - 3, colorCurrent );
    k++;
  }

  FastLED.show();
}

void hue_change(){
    to_target_color();
    to_sides_target_color();
  }

void random_color_per_led(){
   FastLED.clear();
  random16_add_entropy(analogRead(0));
  for (int i = 0; i < 11; i++)
  {
      uint8_t r = random(0,22);
      leds[i] = color_choice[r];
   }
  delay(800);
  FastLED.show();
}

void turn_red_glittery(){
  fract8 chanceOfGlitter = 80;
  uint8_t gHue = 0; 

  for (int i = 0; i < 11; i++){
      leds[i] = CRGB::Blue;
    }
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::Red;
  }
  FastLED.show();  
  FastLED.delay(10000/FRAMES_PER_SECOND); 
 }
void breath_slow(){
  
  float breath = (exp(sin(millis() / 4500.0*PI)) - 0.36787944)*108.0;
  breath = map(breath, 0, 255, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  FastLED.setBrightness(breath);
  fill_rainbow(leds, NUM_LEDS, (hue++/divisor));
  if(hue == (255 * divisor)) {
     hue = 0;
  }
  delay(5);
}


void setup()
{
   EEPROM.write(1,1);//this line needs to be run once the 1st time the code is uploaded and then commented out and re-uploaded.
   mode = EEPROM.read(1);
   if (mode == 1)
    EEPROM.write(1,2);
  else if (mode == 2)
    EEPROM.write(1,3);
  else if (mode == 3)
    EEPROM.write(1,4);
  else if (mode == 4)
    EEPROM.write(1,5);
  else if (mode == 5)
    EEPROM.write(1,6);
  else if (mode == 6)
    EEPROM.write(1,7);
  else if (mode == 7)
    EEPROM.write(1,8);
  else if (mode == 8)
    EEPROM.write(1,1);
   LEDS.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS);
   FastLED.setBrightness(MAX_BRIGHTNESS);
   random16_add_entropy(analogRead(0));
   FastLED.clear();
    
}

void loop() {
    if (mode == 1)
      breath_slow();
    else if (mode == 2)
      turn_red_glittery();
    else if (mode == 3)
      random_color_per_led();
    else if (mode == 4)
      hue_change();
    else if (mode == 5)
        juggle();
    else if (mode == 6)
      solid_red();
    else if (mode == 7)
      solid_blue();
    else if (mode == 8)
      solid_green();
    
  FastLED.show();
  // put your main code here, to run repeatedly:

}
