/**
 * Program      pulseGenerator.cpp
 * 
 * Author       2021-04-05 Charles Geiser (https://www.dodeka.ch)
 * 
 * Purpose      Generates square wave pulses by a simple modulo operation on the micros() counter:
 * 
 *              👉 digitalWrite(pin, (micros() - phase) % period < pulsewidth ? HIGH : LOW) 👈
 * 
 *              The code shows that we can vary period and pulsewidth in a wide range. The phase
 *              is useful when we use more than one pulse generator.  
 *              
 * Board        Arduino Uno, Wemos D1, ESP32 DoIt DevKit V1
 *
 * Wiring       LEDs from phase pins to ground
 * 
 * Remarks      In theory we can get a minimal period of 2 us and a minimal pulsewidth of 1 us 
 *              but in praxis the modulo operation, the digitalWrite() function and the main loop
 *              are continuously called and take a lot of time. Further the Arduino reference says 
 *              that the micros() counter has at best a resolution of 4 us.
 *              With an Arduino Uno some 100 Hz with tolerable jitter are realistic but with an
 *              ESP32 the limit is much higher.
 *                 pros: - simple implementation in one line of code.
 *                       - very long periodes achievable
 *                       - synchronized but phase-shifted signals can be easily generated
 *                 cons: - at frequencies > 100 Hz the jitter is considerable
 *
 * References   I don't know who was the first to publish a blinker with this modulo approach but it
 *              seams to be around as long as the arduino exists.
 *              https://forum.arduino.cc/index.php?topic=360714.0
 *              https://urish.medium.com/5-ways-to-blink-an-led-with-arduino-8f8a41b4fc7d
 */

#include <Arduino.h>

// Activate one of the three examples and see how they work

/*
// Example 1 (2 pulse trains with different frequencies at 2 different pins)
//          ______              ______    
// phase 0 |      |____________|      |__________  2sec / 500ms    
//            __        __        __        __
// phase 1 __|  |______|  |______|  |______|  |__  1sec / 300ms
//
const uint8_t PIN_PHASE_0 = D3;
const uint8_t PIN_PHASE_1 = D4; // = LED_BUILTIN

const uint32_t period0     = 2000000;
const uint32_t period1     = 1000000;
const uint32_t pulsewidth0 = 500000;
const uint32_t pulsewidth1 = 300000;

constexpr uint32_t phase_0 = 0;
constexpr uint32_t phase_1 = 100000;

void setup() 
{
  pinMode(PIN_PHASE_0, OUTPUT);
  pinMode(PIN_PHASE_1, OUTPUT);
}

void loop() 
{
  digitalWrite(PIN_PHASE_0, (micros() - phase_0) % period0 < pulsewidth0 ? HIGH : LOW);
  digitalWrite(PIN_PHASE_1, (micros() - phase_1) % period1 < pulsewidth1 ? HIGH : LOW);
}
*/

/*
// Example 2 (3 phase shifted pulse trains at 3 different pins)
//
// 3-phase generator
//          _____       _____       _____    
// Phase 0 |     |_____|     |_____|     |_____|    0°
//              _____       _____       _____
// Phase 1 ____|     |_____|     |_____|     |__  120°
//         __       _____       _____       ____
// Phase 2   |_____|     |_____|     |_____|      240°
//
const uint8_t PIN_PHASE_0 = D3;
const uint8_t PIN_PHASE_1 = D4; // = LED_BUILTIN
const uint8_t PIN_PHASE_2 = D5;

const uint32_t period     = 3000000;
const uint32_t pulsewidth = 1500000;

constexpr uint32_t phase_0 = 0;
constexpr uint32_t phase_1 = period / 3;
constexpr uint32_t phase_2 = 2 * period / 3;

void setup() 
{
  pinMode(PIN_PHASE_0, OUTPUT);
  pinMode(PIN_PHASE_1, OUTPUT);
  pinMode(PIN_PHASE_2, OUTPUT);
}

void loop() 
{
  digitalWrite(PIN_PHASE_0, (micros() - phase_0) % period < pulsewidth ? HIGH : LOW);
  digitalWrite(PIN_PHASE_1, (micros() - phase_1) % period < pulsewidth ? HIGH : LOW);
  digitalWrite(PIN_PHASE_2, (micros() - phase_2) % period < pulsewidth ? HIGH : LOW);
}
*/

/**/
// Example 3 (2 combined pulse trains at one pin, buzzer = chn1 & chn2)
//          _      _      _      _      _      _      _      _      _      _      _      _      _
// chn1   _| |____| |____| |____| |____| |____| |____| |____| |____| |____| |____| |____| |____| |_
//          ___________________________                                                         ___
// chn2   _|           1sec            |________________________2sec___________________________|
//          _      _      _      _                                                              _
// buzzer _| |____| |____| |____| |____________________________________________________________| |_ 
//        4 pulses in 1sec with 2secs pause
//
const uint8_t PIN_BUZZER = D4;

// Generates pulses (period, pulsewith) and sets the variable chn accordingly
void pulse(bool &chn, uint32_t period, uint32_t pulseWidth)
{
  chn = micros() % period < pulseWidth ? true : false;
}

// Generates 2 pulse trains with period1/pulsewidth1 resp. period2/pulsewidth2,
// combines both with a logical AND operation and sets the pin accordingly
void buzzer(uint8_t pin, uint32_t per1, uint32_t pw1, uint32_t per2, uint32_t pw2)
{
  static bool chn1, chn2;
  pulse(chn1, per1, pw1);
  pulse(chn2, per2, pw2);
  digitalWrite(pin, (chn1 & chn2) ? HIGH : LOW);
}

void setup()
{
  pinMode(PIN_BUZZER, OUTPUT);
}

void loop() 
{
  buzzer(PIN_BUZZER, 250000, 50000, 3000000, 1000000);
}
/**/