#include <Arduino.h>
#include <FastLED.h>
#include "Dosenrohr.h"
#include "Unterboden.h"


#define LED_PIN_Rohr_1 27
#define LED_PIN_Rohr_2 32
#define LED_PIN_Unterboden 33
#define NUM_LEDS_Rohr_1 33
#define NUM_LEDS_Rohr_2 33
#define NUM_LEDS_Unterboden 64

byte colors[3][3] = { {0xff,    0,    0},
                      {0xff, 0xff, 0xff},
                      {   0,    0, 0xff} };

void Thread1(void *);
void Thread2(void *);
void Thread3(void *);

// Instanzen der Klassen erstellen
auto rohr1 = new Dosenrohr<LED_PIN_Rohr_1, NUM_LEDS_Rohr_1>();
auto rohr2 = new Dosenrohr<LED_PIN_Rohr_2, NUM_LEDS_Rohr_2>();
auto unterboden = new Unterboden<LED_PIN_Unterboden, NUM_LEDS_Unterboden>();

// Dosenrohr<LED_PIN_Rohr_2, NUM_LEDS_Rohr_2> rohr2;
// // Dosenrohr<LED_PIN_Rohr_3, NUM_LEDS_Rohr_3> rohr3 = Dosenrohr<LED_PIN_Rohr_3, NUM_LEDS_Rohr_3>();
// Unterboden<LED_PIN_Unterboden, NUM_LEDS_Unterboden> unterboden;


void setup () {

  Serial.begin(9600);

  FastLED.setBrightness(100);

   xTaskCreate(
    Thread1,    // Function that should be called
    "Task 1",   // Name of the task (for debugging)
    1500,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );

  xTaskCreate(
    Thread2,    // Function that should be called
    "Task 2",   // Name of the task (for debugging)
    1500,            // Stack size (bytes)
    NULL,            // Parameter to pass
    2,               // Task priority
    NULL             // Task handle
  );

  xTaskCreate(
    Thread3,    // Function that should be called
    "Task 3",   // Name of the task (for debugging)
    1500,            // Stack size (bytes)
    NULL,            // Parameter to pass
    3,               // Task priority
    NULL             // Task handle
  );
 
}


void loop () {

}


void Thread1(void * parameter){
  for(;;){
    
    // rohr1->shine();
    // rohr1->acceleration();
    // rohr1->runningLightGroup(0xff, 0, 0, 50);
    rohr1->Fire(55,120,15);

  }
}


void Thread2(void * parameter){
  for(;;){

    // rohr2->shine();
    // rohr2->acceleration();
    rohr2->bouncingColoredBalls(3, colors);

  }
}

void Thread3(void * parameter){
  for(;;){

    unterboden->acceleration();
    
  }
}
