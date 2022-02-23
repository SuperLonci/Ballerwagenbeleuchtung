#include <Arduino.h>
#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDSprites.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include "Dosenrohr.h"
#include "Unterboden.h"
#include "Matrix.h"


#define LED_PIN_Rohr_1 25
#define LED_PIN_Rohr_2 26
#define LED_PIN_Unterboden 27
#define LED_PIN_Matrix_1 12
#define LED_PIN_Matrix_2 14


#define NUM_LEDS_Rohr_1 33
#define NUM_LEDS_Rohr_2 33
#define NUM_LEDS_Unterboden 64

#define PIN_Toggle_Switch_1 15
#define PIN_Toggle_Switch_2 2
#define PIN_Toggle_Switch_3 4

#define PIN_Modus_Switch_1 18
#define PIN_Modus_Switch_2 19
#define PIN_Modus_Switch_3 21

boolean Toggle_Switch_1 = false;
boolean Toggle_Switch_2 = false;
boolean Toggle_Switch_3 = false;

boolean Modus_Switch_1 = false;
boolean Modus_Switch_2 = false;
boolean Modus_Switch_3 = false;


byte colors[3][3] = { {0xff,    0,    0},
                      {0xff, 0xff, 0xff},
                      {   0,    0, 0xff} };

void Thread0(void *);
void Thread1(void *);
void Thread2(void *);
void Thread3(void *);
void Thread4(void *);
void Thread5(void *);

// Instanzen der Klassen erstellen
auto rohr1 = new Dosenrohr<LED_PIN_Rohr_1, NUM_LEDS_Rohr_1>();
auto rohr2 = new Dosenrohr<LED_PIN_Rohr_2, NUM_LEDS_Rohr_2>();
auto unterboden = new Unterboden<LED_PIN_Unterboden, NUM_LEDS_Unterboden>();
auto matrix1 = new Matrix<LED_PIN_Matrix_1>();
auto matrix2 = new Matrix<LED_PIN_Matrix_2>();

// Dosenrohr<LED_PIN_Rohr_2, NUM_LEDS_Rohr_2> rohr2;
// // Dosenrohr<LED_PIN_Rohr_3, NUM_LEDS_Rohr_3> rohr3 = Dosenrohr<LED_PIN_Rohr_3, NUM_LEDS_Rohr_3>();
// Unterboden<LED_PIN_Unterboden, NUM_LEDS_Unterboden> unterboden;


void setup () {

  Serial.begin(9600);

  //input Pin Deklaration
  pinMode(PIN_Toggle_Switch_1, INPUT);
  pinMode(PIN_Toggle_Switch_2, INPUT);
  pinMode(PIN_Toggle_Switch_3, INPUT);

  pinMode(PIN_Modus_Switch_1, INPUT);
  pinMode(PIN_Modus_Switch_2, INPUT);
  pinMode(PIN_Modus_Switch_3, INPUT);

  FastLED.setBrightness(32);
  FastLED.clear(true);
  FastLED.show();

   xTaskCreate(
    Thread0,         // Function that should be called
    "Task 0",        // Name of the task (for debugging)
    1500,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );

  xTaskCreate(
    Thread1,         // Function that should be called
    "Task 1",        // Name of the task (for debugging)
    1500,            // Stack size (bytes)
    NULL,            // Parameter to pass
    3,               // Task priority
    NULL             // Task handle
  );
  
    xTaskCreate(
    Thread2,         // Function that should be called
    "Task 2",        // Name of the task (for debugging)
    1500,            // Stack size (bytes)
    NULL,            // Parameter to pass
    3,               // Task priority
    NULL             // Task handle
  );

  xTaskCreate(
    Thread3,         // Function that should be called
    "Task 3",        // Name of the task (for debugging)
    1500,            // Stack size (bytes)
    NULL,            // Parameter to pass
    3,               // Task priority
    NULL             // Task handle
  ); 

  xTaskCreate(
    Thread4,         // Function that should be called
    "Task 4",        // Name of the task (for debugging)
    6000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    3,               // Task priority
    NULL             // Task handle
  ); 

  xTaskCreate(
    Thread5,         // Function that should be called
    "Task 5",        // Name of the task (for debugging)
    6000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    3,               // Task priority
    NULL             // Task handle
  ); 
}


void loop () {

}

void Thread0(void * parameter){
  for(;;){

    Toggle_Switch_1 = digitalRead(PIN_Toggle_Switch_1);
    Toggle_Switch_2 = digitalRead(PIN_Toggle_Switch_2);
    Toggle_Switch_3 = digitalRead(PIN_Toggle_Switch_3);

    Modus_Switch_1 = digitalRead(PIN_Modus_Switch_1);
    Modus_Switch_2 = digitalRead(PIN_Modus_Switch_2);
    Modus_Switch_3 = digitalRead(PIN_Modus_Switch_3);

    delay(10);
    
  }
}

void Thread1(void* parameter){
  for(;;){
    if(!Toggle_Switch_1){
      if(Modus_Switch_1){

        unterboden->regenbogen_cycle(20);
        // delay(10);
        
      } else {
        
        unterboden->dualColorRangeRotation();
        // delay(10);

      }
    } else {

      unterboden->clear();
      // delay(10);

    }
  }
}

void Thread2(void* parameter){
  for(;;){
    if(Toggle_Switch_2){
      if(Modus_Switch_2){

        // rohr1->shine();
        // rohr1->runningLightGroup(0xff, 0, 0, 50);
        rohr1->acceleration();

      } else {

        rohr1->Fire(55,120,15);

      }
    } else {

      rohr1->clear();
      // delay(10);

    }
  }
}

void Thread3(void* parameter){
  for(;;){
    if(Toggle_Switch_2){
      if(Modus_Switch_2){

        // rohr2->shine();
        // rohr2->bouncingColoredBalls(3, colors);
        rohr2->acceleration();

      } else {

        rohr2->meteorRain(0xff,0xff,0xff,10, 64, true, 30);

      }
    } else {

      rohr2->clear();
      // delay(10);

    }
  }
}

void Thread4(void* parameter){
  for(;;){
    if(Toggle_Switch_3){
      if(Modus_Switch_3){

        matrix1->loop(rainbow_plasma, smiley);

      } else {

        matrix1->loop(rainbow_stripe, smiley);
        
      }
    } else {

      matrix1->clear();
      // delay(10);

    }
  }
}

void Thread5(void* parameter){
  for(;;){
    if(Toggle_Switch_3){
      if(Modus_Switch_3){

        matrix2->loop(rainbow_plasma, smiley);

      } else {

        matrix2->loop(rainbow_stripe, smiley);
        
      }
    } else {

      matrix1->clear();
      // delay(10);

    }
  }
}