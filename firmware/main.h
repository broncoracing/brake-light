// CAN Heartbeat message rate in milliseconds. The
#define HEARTBEAT_RATE     100ms

// Byte to read from the steering wheel CAN message for DRS setting.
#define STEERING_WHEEL_DRS_BYTE 0

// Byte to read from brake pressure sensor CAN message for brake pressure.
#define BRAKE_PRESSURE_BYTE 0
// Minimum brake pressure value to turn on the brake light.
#define BRAKE_LIGHT_PRESSURE_CUTOFF 80 // kPa

// Brightness settings for the brake light. 1.0f => full brightness, 0.0f => off

// Default brightness for the brake light
#define DEFAULT_BRIGHTNESS 0.25f
// Minimum brightness setting before cycling back to default
#define MIN_BRIGHTNESS 0.005f


// Uncomment to enable DRS servos.
#define DRS_ENABLED
#ifdef DRS_ENABLED
// Initialize the servos as outputs
// The configuration for the Nucleo-F445RE doesn't support PWM on pins PA2 and PA3 in MBed.
// This is not an issue for the f412 we use on the actual board.
#define NUM_SERVOS 4
PwmOut servos[NUM_SERVOS] = {PwmOut(PA_0), PwmOut(PA_1), PwmOut(PA_2), PwmOut(PA_3)};

// Servo positions when DRS is not active (positions in uS, most servos expect 1000-2000 uS range)
int servo_pos_no_drs[NUM_SERVOS] = {1000, 1000, 1000, 1000};
// Servo positions when DRS is active
int servo_pos_drs[NUM_SERVOS] = {2000, 2000, 2000, 2000};
#endif


// Uncomment to use the LED on the Nucleo dev board instead of the LED pin on the brakelight board.
//#define NUCLEO_LED

// Uncomment to enable debug messages over serial. This will not work on the brake light board since it doesn't have a UART port
//#define PRINT_DEBUG

// Used to disable printf using the macro above
#ifndef PRINT_DEBUG
    #define printf(fmt, ...) (0)
#endif

