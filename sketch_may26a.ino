#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"

uint8_t touch_flag = 0;
uint8_t press_flag = 1;
uint8_t mp3_flag = 0;
uint8_t current_mp3 = -1;

Adafruit_MPR121 cap;
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void setup() {
  Serial.begin(9600);     

  //ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ터치 센서 초기화ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
  if(touch_flag > 0)
  {
    while (!Serial)
    { // needed to keep leonardo/micro from starting too fast!
      delay(10);
    }

    cap = Adafruit_MPR121();

    if (!cap.begin(0x5A))
    {
      Serial.println("MPR121 not found, check wiring?");
      while (1);
    }
    Serial.println("MPR121 found!");
  }

  //ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡdf 플레이어 초기화ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
  if(mp3_flag > 0)
  {
    mp3_set_serial(Serial); // DFPlayer-mini mp3 module 시리얼 세팅
    delay(1);               // 볼륨값 적용을 위한 delay
    mp3_set_volume(30);     // 볼륨조절 값 0~30
  }
}

void loop() {

   if(touch_flag > 0)
   {
     currtouched = cap.touched();

      for (uint8_t i=0; i<12; i++)
      {
        if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) )
        {          
          Serial.print(i); Serial.println(" touched");
          if(mp3_flag > 0)
          {
            if( current_mp3 != i)
            {
              Serial.print("nwe play");
              current_mp3 = i;
              mp3_play (current_mp3 * 3 + 1);    //0002 파일 플레이 , next
            }
            
          }
        }
        if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) )
        {
          Serial.print(i); Serial.println(" released");
          current_mp3  = -1;
        }
      }

      lasttouched = currtouched;
   }

    
 
  
  
  int r0 = analogRead(A0);
 // int r1 = analogRead(s1);

  if(1)
  {
    Serial.print("a0 :");
    Serial.print(r0);
    Serial.print("\n");
  }
  /*

  //if(r1 != 0)
  //{
   // Serial.print("a1 :");
    //Serial.print(r1);
  //  Serial.print("\n");
 // }
  delay(100);
    */

     
    // put your main code here, to run repeatedly:
    // Serial.print("0015 play");
    //mp3_play (15);    //0002 파일 플레이 , next
    //delay(3000);
}
