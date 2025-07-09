#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

// PWM wrap value for the servo control
#define SERVO_PWM_WRAP 40000

// Default system clock frequency (Hz)
#ifndef SYSTEM_CLOCK
#define SYSTEM_CLOCK 125000000
#endif

/**
 * @pin: GPIO pin connected to the servo, must support hardware PWM
 * @angle: Current angle of the servo in degrees
 * @period: PWM signal period (us)
 * @min_duty: Duty cycle at 0 degree (us)
 * @max_duty: Duty cycle at 180 degree (us)
 */
typedef struct servo {
    uint pin, min_duty, max_duty, period;
    float angle;
} servo;

/**
 * Macro to set information of servo from source.
 *
 * @destination: Servo to set (servo*)
 * @source: Servo to copy information (servo*)
 */
#define SERVO_INFO_COPY(destination, source)    \
do{                                             \
    destination->min_duty = source->min_duty;   \
    destination->max_duty = source->max_duty;   \
    destination->period = source->period;       \
}while(0)

/**
 * Macro to select specific servos from an array and store their addresses.
 *
 * @picks: Output array to hold pointers to selected servos (servo**)
 * @servos: Array of all servo instances (servo*)
 * @pick_nums: Array of indexes of servos to pick (uint*)
 * @pick_size: Number of servos to pick (int)
 */
#define PICK_SERVOS(picks, servos, pick_nums, pick_size)            \
do{                                                                 \
    for(int SERVO_ITER = 0; SERVO_ITER < pick_size; SERVO_ITER++) { \
        picks[SERVO_ITER] = &servos[pick_nums[SERVO_ITER]];         \
    }                                                               \
}while(0)

/**
 * Initialize a single servo motor.
 * Make sure all fields in motor are correctly set before calling this.
 * 
 * @motor: Servo to initialize
 */
void servo_init(servo* motor);

/**
 * Set the angle of a single servo motor.
 * 
 * @motor: Servo to set angle
 * @angle: Target angle in degrees
 */
void servo_set(servo* motor, float angle);

/**
 * Move a single servo motor smoothly to the target angle.
 * 
 * @motor: Servo to move
 * @angle: Target angle in degrees
 */
void servo_smooth(servo* motor, float angle);

/**
 * Initialize multiple servo motors.
 * Make sure all servo structs are properly set before calling this.
 * 
 * @number: Number of servos to initialize
 * @motors: Servos to initialize
 */
void servos_init(uint number, servo** motors);

/**
 * Set angles for multiple servos.
 * 
 * @number: Number of servos to set angles
 * @motors: Servos to set angles
 * @angles: Target angles in degrees
 */
void servos_set(uint number, servo** motors, float *angles);

/**
 * Smoothly move multiple servos to target angles.
 * 
 * @number: Number of servos to move
 * @motors: Servos to move
 * @angles: Target angles in degrees
 */
void servos_smooth(uint number, servo** motors, float *angles);


#endif  // SERVO_CONTROL_H