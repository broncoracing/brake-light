/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "CAN.h"
#include "can-ids/CAN_IDS.h"

// Heartbeat message rate in milliseconds
#define HEARTBEAT_RATE     100ms

#define STEERING_WHEEL_DRS_BYTE 0
#define BRAKE_PRESSURE_BYTE 0
#define BRAKE_LIGHT_PRESSURE_CUTOFF 0x80 // TODO Set a reasonable cutoff

// Uncomment to use the LED on the Nucleo dev board instead of the LED pin on the brakelight board.
#define NUCLEO_LED

#define DEFAULT_BRIGHTNESS 1.0f
#define MIN_BRIGHTNESS 0.01f
float brightness = DEFAULT_BRIGHTNESS;


Ticker heartbeat;
CAN can(PA_11, PA_12, 250000);
char counter = 0;

// Initialise the LEDs as a PWM output
#ifdef NUCLEO_LED
PwmOut led(LED1); // Debug led from nucleo board
#else
PwmOut led(PA_8); // Transistor for brake light LED
#endif

// Initialize the servos as outputs
#define NUM_SERVOS 2 // The configuration for the Nucleo-F445RE doesn't support PWM on pins PA2 and PA3 in MBed.
// This can likely be fixed if we need more than 2 independent servos.
PwmOut servos[NUM_SERVOS] = {PwmOut(PA_0), PwmOut(PA_1)};

// Servo positions when DRS is not active (positions in uS, most servos expect 1000-2000 uS range)
int servo_pos_no_drs[NUM_SERVOS] = {1000, 1000};
// Servo positions when DRS is active
int servo_pos_drs[NUM_SERVOS] = {2000, 2000};


// Event queue for handling I/O triggered by an ISR (e.g. printing, sending CAN messages)
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;

// Send out a heartbeat message over CAN
void can_hb() {
    if(can.write(CANMessage(BRAKE_LIGHT, &counter, 1))) {
        counter++;
    } else {
        printf("Failed to send CAN Heartbeat message :(\n");
    }
}

// ISR function - schedules the can heartbeat function to run outside the ISR
 void can_hb_handler() {
    queue.call(can_hb);
}

// Handle CAN frames from the steering wheel
void steering_wheel_received(CANMessage& msg) {
    // Read the value of the DRS button from the steering wheel
    unsigned char drs = msg.data[STEERING_WHEEL_DRS_BYTE];

    printf("Steering wheel DRS setting: %02X\n", (unsigned int)drs);

    // Write to the servos
    if(drs){
        for(int i = 0; i < NUM_SERVOS; i++){
//            servos[i].pulsewidth_us(servo_pos_drs[i]);
        }
    } else {
        for(int i = 0; i < NUM_SERVOS; i++){
//            servos[i].pulsewidth_us(servo_pos_no_drs[i]);
        }
    }
}

// Handle CAN frames from the brake pressure sensor
void brake_pressure_received(CANMessage& msg) {
    unsigned char pressure = msg.data[BRAKE_PRESSURE_BYTE];
    if(pressure > BRAKE_LIGHT_PRESSURE_CUTOFF) {
        led.write(brightness);
    } else {
        led.write(0.0f);
    }
}

void cycle_brightness() {
    // Incrementally decrease brightness until it's below the min, then cycle back to max.
    brightness *= 0.5f;
    if(brightness < MIN_BRIGHTNESS) {
        brightness = 1.0f;
    }

    // If the brake light is currently on, update its brightness.
    if(led.read() != 0) {
        led.write(brightness);
    }
}

// Handle received can frames
void can_received() {
    CANMessage received;
    // Read newly received CAN messages until there aren't any
    while(can.read(received)){
        if(received.id == STEERING_WHEEL_ID) {
            steering_wheel_received(received);
        } else if(received.id == BRAKE_PRESSURE_ID) {
            brake_pressure_received(received);
        } else if(received.id == BRAKE_LIGHT) {
            //  Received message on brake light ID to change brightness
            cycle_brightness();
        }
    }
}

// ISR function which schedules handling of received CAN frames
void can_received_handler() {
    queue.call(can_received);
}

int main()
{
    printf("Entered main\n");

    // Initialize servos
//    for(auto & servo : servos) {
//        servo.period_ms(20); // Most RC servos expect a 20ms period, but this can be sped up for some servos for a faster response
//    }

    // Start the event queue
    t.start(callback(&queue, &EventQueue::dispatch_forever));

    // Attach the callback for the CAN heartbeat message
    heartbeat.attach(&can_hb_handler, HEARTBEAT_RATE);

    // Attach the callback for all received CAN messages
    can.attach(can_received_handler);
}
