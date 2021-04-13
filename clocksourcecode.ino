#include <Adafruit_NeoPixel.h>
#include <stdio.h>
#include <DS1302.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN            8
#define NUMPIXELS      60
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
namespace {


byte yhor;
byte ymin;
byte hor;
byte min;
byte sec;
byte kontrol=0;
 const int kCePin   = 5;  // Chip Enable
const int kIoPin   = 6;  // Input/Output
const int kSclkPin = 7;  // Serial Clock
 
DS1302 rtc(kCePin, kIoPin, kSclkPin);
 

}
 
void printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();
  hor=t.hr;
  min=t.min;
  sec=t.sec;
  if(hor>=12){hor=hor-12;}
  pixels.setPixelColor(5*hor, pixels.Color(25,25,255));
  pixels.setPixelColor(min, pixels.Color(0,200,10));
  pixels.setPixelColor(sec, pixels.Color(255,0,0));
  
  pixels.show();

  pixels.show();
  if(sec==0)
   { 
    pixels.setPixelColor(min-1, pixels.Color(0,0,0));
    pixels.setPixelColor(min, pixels.Color(0,200,10));
     pixels.show();
    for(int i=1;i<60;i++)
      {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
      }
    }

   if(min==0)
  { 
    pixels.setPixelColor(59, pixels.Color(0,0,0));
    }
}


void animasyon()
{
  colorWipe(strip.Color(255, 0, 0), 25); // Red
  colorWipe(strip.Color(0, 255, 0), 25); // Green
  colorWipe(strip.Color(0, 0, 255), 25);
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127, 0, 0), 50); // Red
  theaterChase(strip.Color(0, 0, 127), 50);
  for(int i=1;i<60;i++)
      {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
      }
  
 }
 void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
  void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


  void Setti()
  {
 //kontrol=0;
    hor=yhor;
    min=ymin;
     Time t(2018, 2, 23,yhor, ymin, 0, Time::kFriday);
    rtc.time(t);
    
    }
  
 
 int clockSet =10;
 int up=11;
 int down=12;
void setup() {
  Serial.begin(9600);
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
strip.begin();
  strip.show();
pixels.begin();

pinMode( clockSet,INPUT);

 
  rtc.writeProtect(false);
  rtc.halt(false);
}
 
void loop() {
  if (kontrol==1)
  {
    pixels.setPixelColor(5*yhor, pixels.Color(25,25,255));
    pixels.show();
  delay(100);
  pixels.setPixelColor(5*yhor, pixels.Color(0,0,0));
  pixels.show();
  delay(100);
if (digitalRead(up)==HIGH)
{
  yhor++;
  while(digitalRead(up)==HIGH);
  if(yhor==12){yhor=0;}
}
if (digitalRead(down)==HIGH)
{
  yhor--;
  while(digitalRead(down)==HIGH);
  if(yhor==255){yhor=11;}
}
 
  pixels.setPixelColor(5*yhor, pixels.Color(25,25,255));
  pixels.show();
  delay(100);
  pixels.setPixelColor(5*yhor, pixels.Color(0,0,0));
  pixels.show();
  delay(100);
  Setti();
  if(digitalRead(clockSet)==HIGH)
  { 
  while(digitalRead(clockSet)==HIGH)
  { pixels.setPixelColor(kontrol, pixels.Color(255,255,255));
   pixels.show();
   }
   pixels.setPixelColor(kontrol, pixels.Color(0,0,0));
   pixels.show();
   kontrol++;
   yhor=hor;ymin=min;}
  
    }
 if (kontrol==2)
  {
    if(digitalRead(clockSet)==HIGH)
  { 
    kontrol++;
  while(digitalRead(clockSet)==HIGH)
  {    }
   
   yhor=hor;ymin=min;
   }
   
  if (digitalRead(up)==HIGH)
{
  ymin++;
  while(digitalRead(up)==HIGH);
  if(ymin==60){yhor=0;}
}
if (digitalRead(down)==HIGH)
{
  ymin--;
  while(digitalRead(down)==HIGH);
  if(ymin==255){ymin=59;}
} 
  pixels.setPixelColor(ymin, pixels.Color(0,205,10));
  pixels.show();
  Setti();
  delay(100);
  pixels.setPixelColor(ymin, pixels.Color(0,0,0));
     pixels.show();
     delay(100); 
    }
  if (kontrol==3)
  {
    kontrol=0;
     Setti();
  }
  while (kontrol==0)
  {
    if(digitalRead(up)==HIGH)
  {
    animasyon ();
  }
    if(digitalRead(clockSet)==HIGH)
  { 
    kontrol++;
  while(digitalRead(clockSet)==HIGH);
  yhor=hor;ymin=min;}
  printTime();
  }
}
