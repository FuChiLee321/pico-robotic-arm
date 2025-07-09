#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

// PWM wrap value for the servo control
#define SERVO_PWM_WRAP 40000

// Default system clock frequency (Hz)
#ifndef SYSTEM_CLOCK
#define SYSTEM_CLOCK 125000000
#endif

/**
 * Struct representing a servo motor.
 * 
 * @pin: GPIO pin connected to the servo, must support hardware PWM
 * @angle: Current angle of the servo in degrees
 * @period: PWM signal period (us)
 * @min_duty: PWM duty cycle at 0 degree (us)
 * @max_duty: PWM duty cycle at 180 degree (us)
 */
typedef struct servo {
    uint pin, min_duty, max_duty, period;
    float angle;
} servo;

/**
 * Macro to select specific servos from an array and store their addresses.
 *
 * @picks: Output array to hold pointers to selected servos (servo**)
 * @servos: Array of all servo instances
 * @pick_nums: Array of indices of servos to pick
 * @pick_size: Number of servos to pick
 */
#define PICK_SERVOS(picks, servos, pick_nums, pick_size)            \
do{                                                                 \
    for(int SERVO_ITER = 0; SERVO_ITER < pick_size; SERVO_ITER++) { \
        picks[SERVO_ITER] = &servos[pick_nums[SERVO_ITER]];         \
    }                                                               \
}while(0)

/**
 * Initializes a single servo motor.
 * Make sure all fields in the motor are correctly set before calling this.
 * 
 * @motor: Pointer to a servo struct
 */
void servo_init(servo* motor);

/**
 * Sets the angle of a single servo motor.
 * 
 * @motor: Pointer to a servo struct
 * @angle: Desired angle in degrees
 */
void servo_set(servo* motor, float angle);

/**
 * Moves a single servo motor smoothly to the target angle.
 * 
 * @motor: Pointer to a servo struct
 * @angle: Desired angle in degrees
 */
void servo_smooth(servo* motor, float angle);

/**
 * Initializes multiple servo motors.
 * Make sure all servo structs are properly set before calling this.
 * 
 * @number: Number of servos
 * @motors: Array of pointers to servo structs
 */
void servos_init(uint number, servo** motors);

/**
 * Sets angles for multiple servos at once.
 * 
 * @number: Number of servos
 * @motors: Array of pointers to servo structs
 * @angles: Array of target angles in degrees
 */
void servos_set(uint number, servo** motors, float *angles);

/**
 * Smoothly moves multiple servos to their target angles.
 * 
 * @number: Number of servos
 * @motors: Array of pointers to servo structs
 * @angles: Array of target angles in degrees
 */
void servos_smooth(uint number, servo** motors, float *angles);


#endif  // SERVO_CONTROL_H