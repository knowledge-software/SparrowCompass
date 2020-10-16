#include <Arduino.h>
#include "Waveforms.h"

#define SECONDS 1000

class CompassCtl {

    public:
        CompassCtl(int pulsePin, int dirPin) {
            PWM_PIN = pulsePin;
            DIR_PIN = dirPin;
            twitchDir = 0;
            duration = 0L;
            twitchCurrent = 0;
            dispatch = 0x00;
        }

        // Standard initialization run in the setup() method
        void begin() {
            pinMode(PWM_PIN, OUTPUT);
            pinMode(DIR_PIN, OUTPUT);
            analogWrite(PWM_PIN, 0);
        }

        // Since we are firing a solenoid to redirect the compass 
        // we don't precise control of the direction. What the
        // compass is doing just prior to directional commands
        // can change the reaction of the behavior
        void randomWiggle(int dd=0, bool init=false) {
            static int lastCurrent;

            if (init) { // initialization
                if (dd == -1)
                    twitchDir = false;
                else if (dd == 1)
                    twitchDir = true;
                else
                    twitchDir = random(100)%2 ? true : false;

                duration = millis() + 8*SECONDS;
                twitchCurrent = 1000;
                timerA.setPeriod(1400);
                timerA.setAmplitude(twitchCurrent);
                dispatch |= 0x01;
                lastCurrent = 0;
                return;
            }

            if ( twitchDir )
                digitalWrite(DIR_PIN, LOW);
            else
                digitalWrite(DIR_PIN, HIGH);

            int current = timerA.squareWave();
            analogWrite(PWM_PIN, current);
            if ( lastCurrent > current ) { // when we drop to zero
                twitchDir = !twitchDir;
                lastCurrent = current;
            }


            if (millis()>duration) {
                dispatch ^= 0x01;  
                analogWrite(PWM_PIN, 0);
            }

        }

        void update() {

            if ( dispatch & 0x01 ) 
                randomWiggle();

            // if (millis()>duration) {
            //     digitalWrite(DIR_PIN, HIGH);
            //     analogWrite(DIR_PIN, 0);
            // }
        }



    protected:
        int PWM_PIN;
        int DIR_PIN;
        int twitchCurrent;
        bool twitchDir;
        uint32_t duration;
        uint16_t dispatch;
        Waveforms timerA;
        Waveforms timerB;
};