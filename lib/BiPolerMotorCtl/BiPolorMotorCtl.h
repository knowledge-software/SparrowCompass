/**
 * Class for bipolar motor control using Polulu DRV8838 
 * DC Motor Carrier controller
 * */
#include <Arduino.h>

class BiPolarMotorCtl {

    public:
    
    BiPolarMotorCtl(int pwmPin, int dirPin) {
        pwm_Pin = pwmPin;
        dir_Pin = dirPin;
        // sleep_Pin = sleepPin;
        // steps_Per_Rev = stepsPerRev;
    }

    // traditional way to initialize library in setup() method
    void begin() {
        // Declare pins as output:
        pinMode(pwm_Pin, OUTPUT);
        pinMode(dir_Pin, OUTPUT);
        // pinMode(sleep_Pin, OUTPUT);
        sleep(); 
    }

    // shut off motor (compatability with stepper motor controllers)
    void sleep() {
        analogWrite(pwm_Pin, 0);
    }

    // do nothing as we don't need to emergize (compatability with stepper motor controllers)
    void wake() {
        //digitalWrite(sleep_Pin, HIGH);  // energize 
    }

    // run at speed for a time that is provided 
    // NB this is a blocking routine
    // TODO: Build a non-blocking version
    void run(int speed, uint32_t wait) {
        wake();
        if (speed<0) 
            digitalWrite(dir_Pin, HIGH);
        else
            digitalWrite(dir_Pin, LOW);

        analogWrite(pwm_Pin, abs(speed));
        delay(wait);
        sleep();
    }

    protected:
        int pwm_Pin;
        int dir_Pin;
        int sleep_Pin;
        int steps_Per_Rev;

        uint32_t currLocation = 0;
};
