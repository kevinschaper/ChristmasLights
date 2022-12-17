#include <Arduino.h>
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 200
#define DATA_PIN 9
//#define INITIAL_BRIGHTNESS 30
#define MIN_BRIGHTNESS 1
#define MAX_BRIGHTNESS 255
#define BRIGHTNESS  150

//for fire
#define COOLING  70
#define SPARKING 50
#define FRAMES_PER_SECOND 30
bool gReverseDirection = true;

CRGB leds[NUM_LEDS];

CRGBPalette16 gPal;

void fadeall(int rate) {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(rate);
  }
}

void bright() {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i] = CRGB::White;
  }
  FastLED.show();
  delay(5000);
}

void sparkles(bool purple) {

  int randoSize = random8(NUM_LEDS/2);
  int randomLeds[randoSize];

  for (int i = 0; i < randoSize ; i++ ) {
      randomLeds[i] = random8(NUM_LEDS-1);
      if (purple) {
        leds[randomLeds[i]].setRGB(68, 5, 186);
      } else {
        leds[randomLeds[i]].setRGB(random8(), random8(), random8());
      }

  }


  FastLED.show();
  delay(200);
  fadeall(200);

  FastLED.show();

}

void sparkles() {
  sparkles(false);
}

CRGB redOrGreen() {
  CRGB color = CRGB::Red;
  if (random8(0,100) < 50) {
    color = CRGB::Green;
  }
  return color;
}

CRGB redOrGreenOrPurple() {
  CRGB color = CRGB::Red;
  if (random8(0,100) < 50) {
    color = CRGB::Green;
  }
  if (random8(0,100) > 90) {
    color = CRGB::Purple;
  }
  return color;
}


void xmas() {

  int randoSize = random8(NUM_LEDS/10) % 256 ;
  int randomLeds[randoSize];

  for (int i = 0; i < randoSize ; i++ ) {
      randomLeds[i] = random8(NUM_LEDS-1)  % 256 ;
      leds[randomLeds[i]] = redOrGreenOrPurple();
  }

  FastLED.show();
  delay(200);

  //fade all
  for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8_video(200); }

  FastLED.show();

}


void xmasplosion() {

  int delayTime = 20;
  int fadeRate = 2000;//random8(500,500);

  for (int i = 0 ; i <= (NUM_LEDS-1)/2 ; i++) {
      int bottom = i;
      int top = (NUM_LEDS-1) - i;

      leds[bottom] = CRGB::Red;
      leds[top] = CRGB::Green;

      FastLED.show();
      delay(delayTime);

      fadeall(fadeRate);

      FastLED.show();
      delay(delayTime);
  }

  for (int j = 2 ; j > 0 ; j--) {
    for (int i = (NUM_LEDS-1)/2 ; i >= 0 ; i--) {
      int bottom = i;
      int top = (NUM_LEDS-1) - bottom;

      leds[top]    = CRGB::Purple;
      leds[bottom] = CRGB::Purple;

      FastLED.show();
      delay(j*5);
      fadeall(500);
      FastLED.show();
      delay(j*5);
    }
  }

}

void xmasChase() {

  CRGB color = redOrGreenOrPurple();

  int delayTime = random8(25,50);
  int fadeRate = 2000;//random8(500,500);

  if (color.red > 0) {
      FastLED.setBrightness(200);
  }

  if (random8(1,11) > 2) {
    for (int i = 0 ; i <= NUM_LEDS-1 ; i++) {
        leds[i] = color;
        FastLED.show();
        fadeall(fadeRate);
        FastLED.show();
        delay(delayTime);
    }
  } else {
    for (int i = NUM_LEDS-1 ; i >= 0 ; i--) {
        leds[i] = color;
        FastLED.show();
        fadeall(fadeRate);
        FastLED.show();
        delay(delayTime);
    }
  }


  FastLED.setBrightness(BRIGHTNESS);
}




void Fire2012WithPalette()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

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
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}

void fireLoop() {
  random16_add_entropy( random());
  Fire2012WithPalette(); // run simulation frame, using palette colors
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}




void setup() {
  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  LEDS.setBrightness(BRIGHTNESS);

}


void loop() {

  if (random8(1,10) == 7) {
    xmasplosion();
  }

  for (int i = 1; i < random8(0,3) ; i++) {
    xmasChase();
  }

  //sparkles();
  //bright();

  //for (int i = 1; i < 1000; i++) {
  //  fireLoop();
  //  }

  for (int i = 1; i < random8(10,100); i++) {
    xmas();
  }

}
