
// Yoga Breathalyzer. Using Arduino, WS2812/Neopixel-style LED strip, and Modern Device wind sensor:http://moderndevice.com/product/wind-sensor/

#include "FastSPI_LED2.h" //library for driving LED strips is here: http://code.google.com/p/fastspi/

#define NUM_LEDS 120
struct CRGB leds[NUM_LEDS]; //holds RGB values for the LEDs

// set up max and min values for wind and temp sensors (both built into the Modern Device sensor)
int windmin;
int windmax;
int tempmin;
int tempmax;


void setup() {
  LEDS.setBrightness(250); // reduce total brightness of LED strip to reduce power consumption
  LEDS.addLeds<WS2811, 5>(leds, NUM_LEDS); // set up LED strip on pin 5
  colorFill(10,10,NUM_LEDS,leds);     //fill strip with color while waiting to calibrate
  delay(10000);                       //wait 10 seconds for temp sensor to stabilize
   Serial.begin(9600);
  windmin = analogRead(A0); // calibrate wind sensor
  windmax = windmin+300;    //this is a reasonable range....
  tempmax = analogRead(A2); // calibrate temp sensor
  tempmin = tempmax - 30;   //this is a reasonable range....
}


void loop() {
  int windSensor = analogRead(A0); // read wind sensor value
  int tempSensor = analogRead(A2); // read temp sensor value
 Serial.print(windSensor);
 Serial.print("    ");
 Serial.println(tempSensor);
  
  if(windSensor<windmin) windSensor=windmin; // compress ends of scale to avoid going out of range
  if(windSensor>windmax) windSensor=windmax;

  if(tempSensor<tempmin) tempSensor=tempmin;
  if(tempSensor>tempmax) tempSensor=tempmin;
  
  int dist = map(windSensor, windmin,windmax,0,119); //remap wind value to length of array
  int colorval = map(tempSensor, tempmin,tempmax,0,250); //remap temp to some reasonable color values

 colorFill(colorval,10,dist,leds);     //fill strip with color
 for (int i=0; i < NUM_LEDS; i++) { //clear array 
   memset(leds, 0,  NUM_LEDS * sizeof(struct CRGB));
  }

 }
 
 
// Fill the strip with color c, up to a point determined by the IR sensor value.
void colorFill(uint32_t c, uint8_t wait, uint8_t distance, struct CRGB *leds) {
  int i;
 
  for (i=0; i <= distance; i++) {
      leds[i] = Wheel(c); // set all active pixels on
  }
    LEDS.show();
  delay(wait);
 }

/* Blue-green Color wheel helper function */
//Input a value 0 to 256 to get a color value.
CRGB Wheel(uint16_t WheelPos) {
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      b = 127 - WheelPos % 128; // blue down
      g = WheelPos % 128;       // green up
      r = 0;                    // red off
      break;
    case 1:
      g = 127 - WheelPos % 128; // green down
      b = WheelPos % 128;       // blue up
      r = 0;                    // red off
      break;
   }
  return(CRGB(r, g, b));    // No red here!
}

