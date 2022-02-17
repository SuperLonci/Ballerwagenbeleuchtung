#include <Arduino.h>
#include <FastLED.h>
#include <TaskScheduler.h>
#include "RohrAnimation.h"


#define LED_PIN_Rohr_1 27
#define LED_PIN_Rohr_2 32
#define NUM_LEDS_Rohr_1 70
#define NUM_LEDS_Rohr_2 70

// CRGB leds_Rohr_1[NUM_LEDS_Rohr_1];
// CRGB leds_Rohr_2[NUM_LEDS_Rohr_2];

// Callback methods prototypes
void taskRohr1Callback();
void taskRohr2Callback();
void taskMatrixCallback();

//Tasks
Task taskRohr1(0, TASK_FOREVER, &taskRohr1Callback);
Task taskRohr2(0, TASK_FOREVER, &taskRohr2Callback);
Task taskMatrix(0, TASK_FOREVER, &taskMatrixCallback);

Scheduler scheduler;

RohrAnimation<LED_PIN_Rohr_1> rohr1 = RohrAnimation<LED_PIN_Rohr_1>(70);
RohrAnimation<LED_PIN_Rohr_2> rohr2 = RohrAnimation<LED_PIN_Rohr_2>(70);


void taskRohr1Callback(){
  Serial.print("t1: ");
  Serial.println(millis());

  rohr1.acceleration();

}

void taskRohr2Callback(){
  Serial.print("t1: ");
  Serial.println(millis());

  rohr2.acceleration();

}

void taskMatrixCallback(){

}


void setup () {

  // FastLED.addLeds<NEOPIXEL, LED_PIN_Rohr_2>(leds_Rohr_2, 1);

  Serial.begin(9600);
  Serial.println("Scheduler TEST");
  
  scheduler.init();
  Serial.println("Initialized scheduler");
  
  scheduler.addTask(taskRohr1);
  Serial.println("added taskRohr1");
  
  scheduler.addTask(taskRohr2);
  Serial.println("added taskRohr2");

  delay(5000);
  
  taskRohr1.enable();
  Serial.println("Enabled taskRohr1");
  taskRohr2.enable();
  Serial.println("Enabled taskRohr2");

}


void loop () {
  scheduler.execute();
}
