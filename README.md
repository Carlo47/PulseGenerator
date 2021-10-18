# PulseGenerator
Generates square wave pulses by a simple modulo operation on the micros() counter:

> digitalWrite(pin, (micros() - phase) % period < pulsewidth ? HIGH : LOW)

The code shows that we can vary period and pulsewidth in a wide range. The phase
is useful when we use more than one pulse generator.

In theory we can get a minimal period of 2 us and a minimal pulsewidth of 1 us but in praxis the modulo operation, the digitalWrite() function and the main loop are continuously called and take a lot of time. Further the Arduino reference says that the micros() counter has at best a resolution of 4 us. With an Arduino Uno some 100 Hz with tolerable jitter are realistic but with an ESP32 the limit is much higher.

- pros: 
  - simple implementation in one line of code.
  - very long periodes achievable
  - synchronized but phase-shifted signals can be easily generated
- cons: 
  - at frequencies > 100 Hz the jitter is considerable

I don't know who was the first to publish a blinker with this modulo approach but it seams to be around as long as the Arduino exists.

References: 
- [Arduino Forum](https://forum.arduino.cc/index.php?topic=360714.0)
- [Uri Shaked](https://urish.medium.com/5-ways-to-blink-an-led-with-arduino-8f8a41b4fc7d)

```
Example 1 (2 pulse trains with different frequencies at 2 different pins)
         ______              ______    
phase 0 |      |____________|      |__________  2sec / 500ms    
           __        __        __        __
phase 1 __|  |______|  |______|  |______|  |__  1sec / 300ms
```

```
Example 2 (3 phase shifted pulse trains at 3 different pins)
         _____       _____       _____    
Phase 0 |     |_____|     |_____|     |_____|    0°
             _____       _____       _____
Phase 1 ____|     |_____|     |_____|     |__  120°
        __       _____       _____       ____
Phase 2   |_____|     |_____|     |_____|      240°
```

```
Example 3 (2 combined pulse trains at one pin, buzzer = chn1 & chn2)
         _      _      _      _      _      _      _      _      _      _      _      _      _
chn1   _| |____| |____| |____| |____| |____| |____| |____| |____| |____| |____| |____| |____| |_
         ___________________________                                                         ___
chn2   _|           1sec            |________________________2sec___________________________|
         _      _      _      _                                                              _
buzzer _| |____| |____| |____| |____________________________________________________________| |_ 
       4 pulses in 1sec with 2secs pause
```
