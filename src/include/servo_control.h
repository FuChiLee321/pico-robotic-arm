#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include "pico/stdlib.h"

// PWM wrap value for the servo control
#define SERVO_PWM_WRAP 40000

// Default system clock frequency (Hz)
#ifndef SYSTEM_CLOCK
#define SYSTEM_CLOCK 125000000
#endif

/**
 * @param pin GPIO pin connected to the servo, must support hardware PWM
 * @param angle_range Range of angle the servo can move, usually 180 degrees
 * @param period PWM signal period (us)
 * @param min_duty Duty cycle at 0 degree (us)
 * @param max_duty Duty cycle at 180 degree (us)
 * @param angle Current angle of the servo in degrees
 * @param angle_lower_bound Limit of the lowest angle the servo can move
 * @param angle_upper_bound Limit of the highest angle the servo can move
 */
typedef struct servo {
    uint pin;
    float angle_range;
    uint period;
    uint min_duty;
    uint max_duty;
    float angle;
    float angle_lower_bound;
    float angle_upper_bound;
} servo;

/**
 * Macro to set information of servo from source.
 *
 * @param destination Servo to set (servo*)
 * @param source Servo to copy information (servo*)
 */
#define SERVO_DATASHEET_COPY(destination, source)            \
do{                                                     \
    (destination)->angle_range = (source)->angle_range; \
    (destination)->period = (source)->period;           \
    (destination)->min_duty = (source)->min_duty;       \
    (destination)->max_duty = (source)->max_duty;       \
}while(0)

/**
 * Macro to select specific servos from an array and store their addresses.
 *
 * @param picks Output array to hold pointers to selected servos (servo**)
 * @param servos Array of all servo instances (servo*)
 * @param pick_nums Array of indexes of servos to pick (uint*)
 * @param pick_size Number of servos to pick (int)
 */
#define SERVOS_PICK(picks, servos, pick_nums, pick_size)            \
do{                                                                 \
    for(int SERVO_ITER = 0; SERVO_ITER < pick_size; SERVO_ITER++) { \
        (picks)[SERVO_ITER] = &(servos)[(pick_nums)[SERVO_ITER]];   \
    }                                                               \
}while(0)

/**
 * Initialize a single servo motor.
 * Make sure all fields in motor are correctly set before calling this.
 * 
 * @param motor Servo to initialize
 */
void servo_init(servo* motor);

/**
 * Set GPIO pin of a servo motor.
 * 
 * @param motor Servo to set pin
 * @param pin GPIO pin connected to the servo, must support hardware PWM
 */
void servo_set_pin(servo* motor, uint pin);

/**
 * Set datasheet of a servo.
 * 
 * @param motor Servo to set
 * @param angle_range Range of angle the servo can move, usually 180 degrees
 * @param period PWM signal period (us)
 * @param min_duty Duty cycle at 0 degree (us)
 * @param max_duty Duty cycle at 180 degree (us)
 */
void servo_set_datasheet(servo* motor, float angle_range, uint period, uint min_duty, uint max_duty);

/**
 * Set limits for servo angles.
 * 
 * @param motor Servo to set limits
 * @param angle_lower_bound Limit of the lowest angle the servo can move
 * @param angle_upper_bound Limit of the highest angle the servo can move
 */
void servo_set_limits(servo* motor, float angle_lower_bound, float angle_upper_bound);

/**
 * Set the angle of a single servo motor immediately.
 * 
 * @param motor Servo to set angle
 * @param angle Target angle in degrees
 */
void servo_set_angle(servo* motor, float angle);

/**
 * Move a single servo motor smoothly to the target angle.
 * 
 * @param motor Servo to move
 * @param angle Target angle in degrees
 */
void servo_smooth(servo* motor, float angle);

/**
 * Initialize multiple servo motors.
 * Make sure all servo structs are properly set before calling this.
 * 
 * @param number Number of servos to initialize
 * @param motors Servos to initialize
 */
void servos_init(uint number, servo** motors);

/**
 * Set angles for multiple servos immediately.
 * 
 * @param number Number of servos to set angles
 * @param motors Servos to set angles
 * @param angles Target angles in degrees
 */
void servos_set_angle(uint number, servo** motors, float *angles);

/**
 * Smoothly move multiple servos to target angles.
 * 
 * @param number Number of servos to move
 * @param motors Servos to move
 * @param angles Target angles in degrees
 */
void servos_smooth(uint number, servo** motors, float *angles);


#endif  // SERVO_CONTROL_H