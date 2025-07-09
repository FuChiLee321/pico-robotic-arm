#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "servo_control.h"
#include <math.h>


const uint servo_angle_limit = 0;       // Angle limit for servos in degrees
                                        // Valid servo angle range will be from [limit, 180-limit]

const uint max_servo_move_ms = 2000;    // Maximum time (ms) for a servo smooth move

/**
 * Initializes a single servo motor.
 * Make sure all fields in the motor are correctly set before calling this.
 * 
 * @motor: Pointer to a servo struct
 */
void servo_init(servo* motor) {
    gpio_set_function(motor->pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(motor->pin);
    // 1e6 for convert period (us) to frequency (Hz)
    float clock_devider = (float)SYSTEM_CLOCK / ((float)1e6 / motor->period) / SERVO_PWM_WRAP;
    pwm_set_clkdiv(slice_num, clock_devider);
    pwm_set_wrap(slice_num, SERVO_PWM_WRAP - 1);
    servo_set(motor, motor->angle);
    pwm_set_enabled(slice_num, true);
}

/**
 * Sets the angle of a single servo motor.
 * 
 * @motor: Pointer to a servo struct
 * @angle: Desired angle in degrees
 */
void servo_set(servo* motor, float angle) {
    if(angle < servo_angle_limit)
        angle = servo_angle_limit;
    else if(angle > 180 - servo_angle_limit)
        angle = 180 - servo_angle_limit;
    float duty = (motor->angle / 180) * (motor->max_duty - motor->min_duty) + motor->min_duty;
    uint16_t level = duty / motor->period * SERVO_PWM_WRAP;
    pwm_set_gpio_level(motor->pin, level);
    motor->angle = angle;
}

/**
 * Moves a single servo motor smoothly to the target angle.
 * 
 * @motor: Pointer to a servo struct
 * @angle: Desired angle in degrees
 */
void servo_smooth(servo* motor, float angle) {
    float start_angle = motor->angle;
    float angle_difference = angle - motor->angle;
    uint steps = angle_difference / 180 * max_servo_move_ms / motor->period;
    for(uint step = 1; step < steps; step++) {
        // Calculate the smooth transition ratio using a cosine function for easing effect
        float ratio = 0.5 + cosf(M_PI * step / steps) / 2;
        float delta = angle_difference * ratio;
        servo_set(motor, start_angle + delta);
        sleep_us(motor->period);
    }
    servo_set(motor, angle);
    motor->angle = angle;
}

/**
 * Initializes multiple servo motors.
 * Make sure all servo structs are properly set before calling this.
 * 
 * @number: Number of servos
 * @motors: Array of pointers to servo structs
 */
void servos_init(uint number, servo** motors) {
    for(uint i = 0; i < number; i++) {
        gpio_set_function(motors[i]->pin, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(motors[i]->pin);
        // 1e6 for convert period (us) to frequency (Hz)
        float clock_devider = (float)SYSTEM_CLOCK / ((float)1e6 / motors[i]->period) / SERVO_PWM_WRAP;
        pwm_set_clkdiv(slice_num, clock_devider);
        pwm_set_wrap(slice_num, SERVO_PWM_WRAP - 1);
        servo_set(motors[i], motors[i]->angle);
    }
    for(uint i = 0; i < number; i++) {
        uint slice_num = pwm_gpio_to_slice_num(motors[i]->pin);
        pwm_set_enabled(slice_num, true);
    }
}

/**
 * Sets angles for multiple servos at once.
 * 
 * @number: Number of servos
 * @motors: Array of pointers to servo structs
 * @angles: Array of target angles in degrees
 */
void servos_set(uint number, servo** motors, float *angles) {
    for(uint i = 0; i < number; i++) {
        servo_set(motors[i], angles[i]);
    }
}

/**
 * Smoothly moves multiple servos to their target angles.
 * 
 * @number: Number of servos
 * @motors: Array of pointers to servo structs
 * @angles: Array of target angles in degrees
 */
void servos_smooth(uint number, servo** motors, float *angles) {
    float start_angles[number];
    float angle_differences[number];
    uint max_steps = 0;
    uint max_period = 1;
    // Store start angles and angle differences for each servo
    // Determine the max steps and max period
    for(uint i = 0; i < number; i++) {
        start_angles[i] = motors[i]->angle;
        angle_differences[i] = angles[i] - start_angles[i];
        uint steps = angle_differences[i] / 180 * max_servo_move_ms / motors[i]->period;
        if(steps > max_steps)
            max_steps = steps;
        if(motors[i]->period > max_period)
            max_period = motors[i]->period;
    }
    // Perform the smooth transition in steps
    for(uint step = 1; step < max_steps; step++) {
        // Calculate the smooth transition ratio using a cosine function for easing effect
        float ratio = 0.5 + cosf(M_PI * step / max_steps) / 2;
        for(uint i = 0; i < number; i++) {
            float delta = angle_differences[i] * ratio;
            servo_set(motors[i], start_angles[i] + delta);
        }
        sleep_us(max_period);
    }
    servos_set(number, motors, angles);
}