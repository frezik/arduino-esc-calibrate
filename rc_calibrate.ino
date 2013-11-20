/*
Copyright (c) 2013, Timm Murray
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Calibrates the HobbyKing X-Car BESC.  Calibration goes through a number of 
 * states:
 *
 * 0) Reverse throttle
 * 1) Full throttle
 * 2) Brake throttle
 * 3) Neutral throttle
 *
 * The BESC blinks and beeps to indicate that it's done in each state.  When it 
 * does that, hit a switch connected to digital pin 7.  If you have an LED on 
 * digital pin 12, it will alternate between high and low to indicate that it 
 * moved to the next state.
 */
#include <Servo.h>

#define SERVO_MIN_DEGREES 0
#define SERVO_MAX_DEGREES 179
#define SERVO_MIN_PULSE_WIDTH 1000
#define SERVO_MAX_PULSE_WIDTH 1000
#define SERVO_PIN 2
#define LED 12
#define BTN 7

#define REVERSE_THROTTLE 0
#define FULL_THROTTLE 179
#define BRAKE_THROTTLE 89
#define NEUTRAL_THROTTLE 30

Servo motor;
int state = 0;
int last_btn_state = HIGH; // Digital pin will be high on first read


void setup()
{
    Serial.begin(9600);

    pinMode( LED, OUTPUT );
    digitalWrite( LED, HIGH );

    pinMode( BTN, INPUT );
    digitalWrite( BTN, HIGH ); // Activate pullups

    motor.attach( SERVO_PIN, SERVO_MIN_PULSE_WIDTH,
        SERVO_MAX_PULSE_WIDTH );
}

void loop()
{
    int cur_btn_state = digitalRead( BTN );
    Serial.print( "Last btn state: " );
    Serial.print( last_btn_state );
    Serial.print( ", cur btn state: " );
    Serial.print( cur_btn_state );
    Serial.print( ", position state: " );
    Serial.println( state );

    if( cur_btn_state != last_btn_state ) {
        // Change state when we're moving from high to low (let off button)
        if( HIGH == cur_btn_state ) 
            state++;
    }
    last_btn_state = cur_btn_state;


    switch(state) {
        case 0:
            motor.write( REVERSE_THROTTLE );
            digitalWrite( LED, HIGH );
            break;
        case 1:
            motor.write( FULL_THROTTLE );
            digitalWrite( LED, LOW );
            break;
        case 2:
            motor.write( BRAKE_THROTTLE );
            digitalWrite( LED, HIGH );
            break;
        case 3:
            motor.write( NEUTRAL_THROTTLE );
            digitalWrite( LED, LOW );
            break;
        default:
            // Do nothing, successfully
            break;
    }
}
