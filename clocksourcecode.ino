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


byte ysaat;
byte ydak;
byte saat;
byte dak;
byte sn;
byte kontrol=0;
 const int kCePin   = 5;  // Chip Enable
const int kIoPin   = 6;  // Input/Output
const int kSclkPin = 7;  // Serial Clock
 
DS1302 rtc(kCePin, kIoPin, kSclkPin);
 

}
 
void printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();
  saat=t.hr;
  dak=t.min;
  sn=t.sec;
  if(saat>=12){saat=saat-12;}
  pixels.setPixelColor(5*saat, pixels.Color(25,25,255));
  pixels.setPixelColor(dak, pixels.Color(0,200,10));
  pixels.setPixelColor(sn, pixels.Color(255,0,0));
  
  pixels.show();
  //Serial.println(t.sec);
  //pixels.setPixelColor(t.hr, pixels.Color(0,0,0));
  //pixels.setPixelColor(t.sec-1, pixels.Color(0,0,0));
  
  pixels.show();
  if(sn==0)
   { 
    pixels.setPixelColor(dak-1, pixels.Color(0,0,0));
    pixels.setPixelColor(dak, pixels.Color(0,200,10));
     pixels.show();
    for(int i=1;i<60;i++)
      {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
      }
    }

   if(dak==0)
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


  void yenisaat()
  {
 //kontrol=0;
    saat=ysaat;
    dak=ydak;
     Time t(2018, 2, 23,ysaat, ydak, 0, Time::kFriday);
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
    pixels.setPixelColor(5*ysaat, pixels.Color(25,25,255));
    pixels.show();
  delay(100);
  pixels.setPixelColor(5*ysaat, pixels.Color(0,0,0));
  pixels.show();
  delay(100);
if (digitalRead(up)==HIGH)
{
  ysaat++;
  while(digitalRead(up)==HIGH);
  if(ysaat==12){ysaat=0;}
}
if (digitalRead(down)==HIGH)
{
  ysaat--;
  while(digitalRead(down)==HIGH);
  if(ysaat==255){ysaat=11;}
}
 
  pixels.setPixelColor(5*ysaat, pixels.Color(25,25,255));
  pixels.show();
  delay(100);
  pixels.setPixelColor(5*ysaat, pixels.Color(0,0,0));
  pixels.show();
  delay(100);
  yenisaat();
  if(digitalRead(clockSet)==HIGH)
  { 
  while(digitalRead(clockSet)==HIGH)
  { pixels.setPixelColor(kontrol, pixels.Color(255,255,255));
   pixels.show();
   }
   pixels.setPixelColor(kontrol, pixels.Color(0,0,0));
   pixels.show();
   kontrol++;
   ysaat=saat;ydak=dak;}
  
    }
 if (kontrol==2)
  {
    if(digitalRead(clockSet)==HIGH)
  { 
    kontrol++;
  while(digitalRead(clockSet)==HIGH)
  {    }
   
   ysaat=saat;ydak=dak;
   }
   
  if (digitalRead(up)==HIGH)
{
  ydak++;
  while(digitalRead(up)==HIGH);
  if(ydak==60){ysaat=0;}
}
if (digitalRead(down)==HIGH)
{
  ydak--;
  while(digitalRead(down)==HIGH);
  if(ydak==255){ydak=59;}
} 
  pixels.setPixelColor(ydak, pixels.Color(0,205,10));
  pixels.show();
  yenisaat();
  delay(100);
  pixels.setPixelColor(ydak, pixels.Color(0,0,0));
     pixels.show();
     delay(100); 
    }
  if (kontrol==3)
  {
    kontrol=0;
     yenisaat();
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
  ysaat=saat;ydak=dak;}
  printTime();
  }
}