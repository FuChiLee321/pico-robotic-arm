#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "servo_control.h"
#include <math.h>

const uint angle_limit = 0;
const uint max_servo_move_ms = 2000;

/**
 * please set data in motor correctly before initialize
 * 
 * servo_init() - initialize servo motor
 * @motor: address of servo motor
 */
void servo_init(servo* motor) {
    gpio_set_function(motor->pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(motor->pin);
    // 1e6 for convert period(us) to frequence
    float clock_devider = (float) SYSTEM_CLOCK / ((float)1e6 / motor->period * SERVO_PWM_WRAP);
    pwm_set_clkdiv(slice_num, clock_devider);
    pwm_set_wrap(slice_num, SERVO_PWM_WRAP - 1);
    servo_set(motor, motor->angle);
    pwm_set_enabled(slice_num, true);
}

/**
 * servo_set() - set angle of servo motor
 * @motor: address of servo motor
 * @angle: target angle
 */
void servo_set(servo* motor, float angle) {
    if(angle < angle_limit)
        angle = angle_limit;
    else if(angle > 180 - angle_limit)
        angle = 180 - angle_limit;
    float duty = (motor->angle / 180) * (motor->max_duty - motor->min_duty) + motor->min_duty;
    uint16_t level = duty / motor->period * SERVO_PWM_WRAP;
    pwm_set_gpio_level(motor->pin, level);
    motor->angle = angle;
}

/**
 * servo_smooth() - smooth move single servo motor to angle
 * @motor: address of servo motor
 * @angle: target angle
 */
void servo_smooth(servo* motor, float angle) {
    float start_angle = motor->angle;
    float angle_difference = angle - motor->angle;
    uint steps = angle_difference / 180 * max_servo_move_ms / motor->period;
    for(uint step = 1; step < steps; step++) {
        float delta = angle_difference * (0.5 + cosf(M_PI * step / steps) / 2);
        servo_set(motor, start_angle + delta);
        sleep_us(motor->period);
    }
    servo_set(motor, angle);
    motor->angle = angle;
}

/**
 * please set data in motors correctly before initialize
 * 
 * servos_init() - initialize servo motors
 * @quantity: quantity of servo motors
 * @motors: array of address of servo motors
 */
void servos_init(uint quantity, servo** motors) {
    for(uint i = 0; i < quantity; i++) {
        gpio_set_function(motors[i]->pin, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(motors[i]->pin);
        // 1e6 for convert period(us) to frequence
        float clock_devider = (float)SYSTEM_CLOCK / ((float)1e6 / motors[i]->period * SERVO_PWM_WRAP);
        pwm_set_clkdiv(slice_num, clock_devider);
        pwm_set_wrap(slice_num, SERVO_PWM_WRAP - 1);
        servo_set(motors[i], motors[i]->angle);
    }
    for(uint i = 0; i < quantity; i++) {
        uint slice_num = pwm_gpio_to_slice_num(motors[i]->pin);
        pwm_set_enabled(slice_num, true);
    }
}

/**
 * servos_set() - set angles of multiple servo motors
 * @quantity: quantity of servo motors
 * @motors: array of address of servo motors
 * @angles: array of target angles
 */
void servos_set(uint quantity, servo** motors, float *angles) {
    for(uint i = 0; i < quantity; i++) {
        servo_set(motors[i], angles[i]);
    }
}

/**
 * servos_smooth() - smooth move multiple servo motors to angles
 * @quantity: quantity of servo motors
 * @motors: array of address of servo motors
 * @angles: array of target angles
 */
void servos_smooth(uint quantity, servo** motors, float *angles) {
    float start_angles[quantity];
    float angle_differences[quantity];
    uint max_steps = 0;
    uint max_period = 1;
    for(uint i = 0; i < quantity; i++) {
        start_angles[i] = motors[i]->angle;
        angle_differences[i] = angles[i] - start_angles[i];
        uint steps = angle_differences[i] / 180 * max_servo_move_ms / motors[i]->period;
        if(steps > max_steps)
            max_steps = steps;
        if(motors[i]->period > max_period)
            max_period = motors[i]->period;
    }
    for(uint step = 1; step < max_steps; step++) {
        for(uint i = 0; i < quantity; i++) {
            float delta = angle_differences[i] * (0.5 + cosf(M_PI * step / max_steps) / 2);
            servo_set(motors[i], start_angles[i] + delta);
        }
        sleep_us(max_period);
    }
    servos_set(quantity, motors, angles);
}