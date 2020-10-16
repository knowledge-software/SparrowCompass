#include <Arduino.h>
#include "LightShow.h"
#include "CompassCtl.h"
#include <Streaming.h>

const int LED_PIN = 2;
const int PWM_PIN = 3;
const int DIR_PIN = 4;

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


/**
 *  Set up 
 */
void setup() {
    Serial.begin(115200);

    ring.begin();
    ring.oceanWaves(true);


    Serial << "READY!" << endl;
}


/**
 *  Run
 */
void loop() {

    char cc = 0;
    if (Serial.available()) { 
        cc = Serial.read();
        ring.warningRed(true);
        Serial << "c: '" << cc << "'" << endl;
        if (cc=='a')
            compass.randomWiggle( 1, true);
        else if (cc=='s')
            compass.randomWiggle(-1, true);
        else
            compass.randomWiggle( 0, true);

    }

    ring.update();
    compass.update();



    // Some example procedures showing how to display to the pixels:
    // Serial << "colorWipe RED" << endl;
    // colorWipe(strip.Color(255, 0, 0), 50); // Red
    // Serial << "colorWipe GREEN" << endl;
    // colorWipe(strip.Color(0, 255, 0), 50); // Green
    // Serial << "colorWipe BLUE" << endl;
    // colorWipe(strip.Color(0, 0, 255), 50); // Blue
    // //colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
    // Serial << "counterclockwise" << endl;

    // Send a theater pixel chase in...
    // Serial << "theaterChase WHITE" << endl;
    // theaterChase(strip.Color(127, 127, 127), 50); // White

    // Serial << "theaterChase RED" << endl;
    // theaterChase(strip.Color(127, 0, 0), 50); // Red
}

