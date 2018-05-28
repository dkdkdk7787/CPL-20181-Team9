#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"

void setup();
void loop();

uint8_t touch_flag = 1;
uint8_t press_flag = 0;
uint8_t mp3_flag = 1;

uint8_t current_mp3 = -1;

int r0 = 0;
int r1 = 0;
int r2 = 0;

int vias = 1;

Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void setup() {
  Serial.begin(9600);    
  //SoftwareSerial mySerial(10, 11); // RX, TX

  while (!Serial)
  { // needed to keep leonardo/micro from starting too fast!
      delay(10);
  }

  Serial.println("initialize start");

   //ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ터치 센서 초기화ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
  if(touch_flag > 0)
  {
    Serial.println("Adafruit_MPR121 end");
    if (!cap.begin(0x5A))
    {
      Serial.println("MPR121 not found, check wiring?");
    }
    Serial.println("MPR121 found!");
  }

  Serial.println("df init start");
  //ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡdf 플레이어 초기화ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
  if(mp3_flag > 0)
  {
    //  mp3_set_serial (mySerial);
    mp3_set_serial(Serial); // DFPlayer-mini mp3 module 시리얼 세팅
    delay(1);               // 볼륨값 적용을 위한 delay
    mp3_set_volume(30);     // 볼륨조절 값 0~30
  }

  
  Serial.println("initialize complete");
}

void loop() {

   if(press_flag > 0)
   {
      r0 = analogRead(A0);
      r1 = analogRead(A1);
      r2 = analogRead(A2);
      
      //if(r0 != 0)
      {
        //Serial.print("a0 :");
       //Serial.print(r0);
        //Serial.print("\n"); 
      }
  
      //if(r1 != 0)
      {
       // Serial.print("a1 :");
        //Serial.print(r1);
        //Serial.print("\n");
      }
       //if(r2 != 0)
      {
        //Serial.print("a2 :");
        //Serial.print(r2);
        //Serial.print("\n");
      }
   }

  
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
              Serial.print("play\n");
              current_mp3 = i;
              
              if(press_flag > 0)
              {
                
                if( i <= 2) //r0 센서
                {
                  if(r0 <= 10)
                    vias = 1;
                  else if( r0 <= 50 )
                    vias = 3;
                  else
                    vias = 6;
                }
                else if( i <= 6) //r1 센서
                {
                  if(r1 <= 20)
                    vias = 1;
                  else if( r1 <= 23 )
                    vias = 3;
                  else
                    vias = 6;
                }
                else if( i <= 10) //r2 센서
                {
                  if(r2 <= 0)
                    vias = 1;
                  else if( r2 <= 2 )
                    vias = 3;
                  else
                    vias = 6;
                }   
                    
              }
             

              mp3_play (17 - (current_mp3 / 2 * 3 + vias) ); 
              
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

    

   
}
