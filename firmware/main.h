// CAN Heartbeat message rate in milliseconds. The
#define HEARTBEAT_RATE     100ms

// Byte to read from the steering wheel CAN message for DRS setting.
#define STEERING_WHEEL_DRS_BYTE 0

// Byte to read from brake pressure sensor CAN message for brake pressure.
#define BRAKE_PRESSURE_BYTE 0
// Minimum brake pressure value to turn on the brake light.
#define BRAKE_LIGHT_PRESSURE_CUTOFF 0x80 // TODO Set a reasonable cutoff

// Brightness settings for the brake light. 1.0f => full brightness, 0.0f => off

// Default brightness for the brake light
#define DEFAULT_BRIGHTNESS 1.0f
// Minimum brightness setting before cycling back to default
#define MIN_BRIGHTNESS 0.01f


// Uncomment to enable DRS servos.
#define DRS_ENABLED

// Uncomment to use the LED on the Nucleo dev board instead of the LED pin on the brakelight board.
//#define NUCLEO_LED

// Uncomment to enable debug messages over serial. This will not work on the brake light board since it doesn't have a UART port
//#define PRINT_DEBUG

// Used to disable printf using the macro above
#ifndef PRINT_DEBUG
    #define printf(fmt, ...) (0)
#endif

