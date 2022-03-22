
#include <Adafruit_NeoPixel.h>


// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN		27

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT	5

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void setup(void)
{
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
}

#define MAX_BRIGHTNESS 64
#define BRIGHTNESS_STEP 4

void loop(void)
{
	static uint8_t red=0;
	static uint8_t green=0;
	static uint8_t blue=0;
	
	static uint32_t count=0;

	if((count%3)==0)	red+=BRIGHTNESS_STEP;
	if((count%5)==0)	green+=BRIGHTNESS_STEP;
	if((count%7)==0)	blue+=BRIGHTNESS_STEP;
	
	if(red>MAX_BRIGHTNESS)		red=0;
	if(green>MAX_BRIGHTNESS)	green=0;
	if(blue>MAX_BRIGHTNESS)		blue=0;

	for(int i=0;i<LED_COUNT;i++)
		strip.setPixelColor(i,red,green,blue);

	count++;
	delay(100);
}

