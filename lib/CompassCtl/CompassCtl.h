#include <Arduino.h>
#include "Waveforms.h"

#if !defined(__AVR_ATtiny85__)
#include <Streaming.h>
#endif

#define SECONDS 1000UL

class CompassCtl {

    public:
        CompassCtl(int pulsePin, int dirPin) {
            PWM_PIN = pulsePin;
            DIR_PIN = dirPin;
            wayOne = HIGH;
            wayTwo = LOW;
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

        // randomWiggle
        void crazyWiggle(bool init=false) {

            if (init) { // initialization
                int dd = (2 * (random(100)%2)) - 1;
                if (dd == toEast) {
                    wayOne = LOW;
                    wayTwo = HIGH;
                }
                else if (dd == toWest) {
                    wayOne = HIGH;
                    wayTwo = LOW;
                }

                duration = millis() + 7*SECONDS;
                twitchCurrent = 180;
                timerB.setPeriod(1300);
                timerB.setAmplitude(2*twitchCurrent);
                timerB.setOffset(-twitchCurrent);
                dispatch |= crazyWiggleFlag;
                //Serial << "crazy" << endl;
                return;
            }

            //if ( millis()<next ) return;
            // drive the eMag
            int current = timerB.squareWave();
            digitalWrite(DIR_PIN, current<0 ? wayOne : wayTwo );
            analogWrite(PWM_PIN, abs(current));

            // Off switch
            if (millis()>duration) {
                dispatch ^= crazyWiggleFlag;  
                analogWrite(PWM_PIN, 0);
                //Serial << " --czy off " << endl;
            }

        }

        // Since we are firing a solenoid to redirect the compass 
        // we don't precise control of the direction. What the
        // compass is doing just prior to directional commands
        // can change the reaction of the behavior
        void randomWiggle(int dd=0, bool init=false) {

            if (init) { // initialization
                if (dd ==  randDir) dd = (2 * (random(100)%2)) - 1;
                if (dd == toEast) {
                    wayOne = LOW;
                    wayTwo = HIGH;
                    twitchCurrent = 95;
                }
                else if (dd == toWest) {
                    wayOne = HIGH;
                    wayTwo = LOW;
                    twitchCurrent = 80;
                }

                duration = millis() + 8*SECONDS;
                timerA.setPeriod(1231);
                timerA.setAmplitude(2*twitchCurrent);
                timerA.setOffset(-twitchCurrent);
                dispatch |= lightWiggleFlag;
                return;
            }

            //if ( millis()<next ) return;
            // drive the eMag
            int current = timerA.squareWave();
            digitalWrite(DIR_PIN, current<0 ? wayOne : wayTwo );
            analogWrite(PWM_PIN, abs(current));

            // Off switch
            if (millis()>duration) {
                dispatch ^= lightWiggleFlag;  
                analogWrite(PWM_PIN, 0);
            }

        }

        // Try poiinting the compass in a particular direction
        void setDir(int dd=0, bool init=false) {

            if (init) { // initialization
                if (dd ==  randDir) dd = (2 * (random(100)%2)) - 1;
                if (dd == toEast) {
                    digitalWrite(DIR_PIN, LOW );
                }
                else if (dd == toWest) {
                    digitalWrite(DIR_PIN, HIGH );
                }
                twitchCurrent = 60;

                duration = millis() + 13*SECONDS;
                dispatch |= setDirectionFlag;

                analogWrite(PWM_PIN, twitchCurrent);
                return;
            }

            // Off switch
            if (millis()>duration) {
                dispatch ^= setDirectionFlag;  
                analogWrite(PWM_PIN, 0);
            }

        }

        void update() {

            if ( dispatch & crazyWiggleFlag ) 
                crazyWiggle();

            if ( dispatch & lightWiggleFlag )
                randomWiggle();

            if ( dispatch & setDirectionFlag )
                setDir(); // will just shut off after time

        }

        void shutDown( int flags ) {
            dispatch &= !flags;          
        }
    public:
        enum CompassSwitches {
            toEast = -1,
            toWest = 1,
            randDir = 0,

            crazyWiggleFlag = 0x04,
            lightWiggleFlag = 0x01,
            setDirectionFlag = 0x02,
        };


    protected:
        int PWM_PIN;
        int DIR_PIN;
        int twitchCurrent;
        int wayOne;
        int wayTwo;
        uint32_t duration;
        uint16_t dispatch;
        Waveforms timerA;
        Waveforms timerB;
};