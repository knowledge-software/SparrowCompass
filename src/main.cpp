#include <Arduino.h>
#include "LightShow.h"
#include "CompassCtl.h"

#define ATtiny85

#if defined(ATtiny85)
const int LED_PIN = 4;
const int PWM_PIN = 0;
const int DIR_PIN = 1;
#else
#include <Streaming.h>
const int LED_PIN = 2;
const int PWM_PIN = 3;
const int DIR_PIN = 4;
#endif

const uint32_t ROTSPD = 500; // pwm
const uint32_t ROTTIM = 500; // ms

//BiPolarMotorCtl  motor = BiPolarMotorCtl(PWM_PIN, DIR_PIN);
CompassCtl compass = CompassCtl(PWM_PIN, DIR_PIN);
LightShow ring(LED_PIN, 50);

#ifdef __AVR__
  #include <avr/power.h>
#endif

// void colorWipe(uint32_t c, uint8_t wait);
// void rainbow(uint8_t wait);
// void rainbowCycle(uint8_t wait);
// void theaterChase(uint32_t c, uint8_t wait);
// void theaterChaseRainbow(uint8_t wait);
// uint32_t Wheel(byte WheelPos);


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
//Adafruit_NeoPixel pixel = Adafruit_NeoPixel(24, LED_PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// Wiring:
// Pin 8 to IN1 on the ULN2003 driver
// Pin 9 to IN2 on the ULN2003 driver
// Pin 10 to IN3 on the ULN2003 driver
// Pin 11 to IN4 on the ULN2003 driver
// Create stepper object called 'myStepper', note the pin order:
//Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);
// const int stepsPerRevolution = 2048;
// AccelStepper stepper = AccelStepper(AccelStepper::HALF4WIRE, 8, 10, 9, 11);

unsigned long wiggleTimer = 0UL;
#define WIGGLE_TRIGGER 3*37*SECONDS * 1/2;

unsigned long crazyTimer = 0UL;
#define CRAZY_TRIGGER 3*157*SECONDS * 1/2;

unsigned long dirTimer = 0UL;
#define DIR_TRIGGER  20*SECONDS;

/**
 *  Set up 
 */
void setup() {

    ring.begin();
    ring.oceanWaves(true);

    pinMode(2, OUTPUT); // special output for scope trigger

#if defined(ATtiny85)
    crazyTimer = millis() + CRAZY_TRIGGER;
    wiggleTimer = millis() + WIGGLE_TRIGGER;
    dirTimer = millis() + DIR_TRIGGER;
#else
    Serial.begin(115200);
    Serial << "READY!" << endl;
#endif
}

#ifdef ATtiny85
// Debugging ATtinys requrie a scope.
// This raises a pin to monitor status
void triggerScope(int pin, int duration) {
    // digitalWrite(pin, HIGH);
    // delay(duration);
    // digitalWrite(pin,LOW);
}
#endif

/**
 *  Run
 */
void loop() {

#if defined(ATtiny85)

    if (millis()>crazyTimer) {
        compass.shutDown(compass.setDirectionFlag|compass.lightWiggleFlag);
        compass.crazyWiggle(true);
        crazyTimer = millis() + CRAZY_TRIGGER;
        ring.warningRed(true);
        triggerScope(2, 3);
    }

    if (millis()>wiggleTimer) {
        compass.shutDown(compass.setDirectionFlag);
        compass.randomWiggle(compass.randDir, true);
        wiggleTimer = millis() + WIGGLE_TRIGGER;
        triggerScope(2, 5);
    }

    if (millis()>dirTimer) { 
        //compass.setDir(compass.randDir, true);
        dirTimer = millis() + DIR_TRIGGER + random(500);
        triggerScope(2, 7);
    }

#else
    char cc = 0;
    if (Serial.available()) { 
        cc = Serial.read();
        ring.warningRed(true);
        Serial << "c: '" << cc << "'" << endl;
        if (cc=='a')
            compass.randomWiggle( 1, true);
        else if (cc=='s')
            compass.randomWiggle(-1, true);
        else if (cc==' ')
            compass.randomWiggle( 0, true);
        else if (cc=='p')
            compass.crazyWiggle(true);
    }
#endif
    ring.update();
    compass.update();

    delay(5);
}

