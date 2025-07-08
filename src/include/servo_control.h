#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H


#define SERVO_PWM_WRAP 40000
#define SERVO_SMOOTH_DELAY_us 20
#ifndef SYSTEM_CLOCK
#define SYSTEM_CLOCK 125000000
#endif

/**
 * @pin: gpio pin with hardware pwm
 * @angle: servo motor angle (degree)
 * @period: period of pwm (us)
 * @min_duty: pwm duty at 0 degree (us)
 * @max_duty: pwm duty at 180 degree (us)
 */
typedef struct servo {
    uint pin, min_duty, max_duty, period;
    float angle;
}servo;


/**
 * please set data in motor correctly before initialize
 * 
 * servo_init() - initialize servo motor
 * @motor: address of servo motor
 */
void servo_init(servo* motor);

/**
 * servo_set() - set angle of servo motor
 * @motor: address of servo motor
 * @angle: target angle
 */
void servo_set(servo* motor, float angle);

/**
 * servo_smooth() - smooth move single servo motor to angle
 * @motor: address of servo motor
 * @angle: target angle
 */
void servo_smooth(servo* motor, float angle);

/**
 * please set data in motors correctly before initialize
 * 
 * servos_init() - initialize servo motors
 * @quantity: quantity of servo motors
 * @motors: array of servo motors
 */
void servos_init(uint quantity, servo* motors) ;

/**
 * servos_set() - set angles of multiple servo motors
 * @quantity: quantity of servo motors
 * @motors: array of servo motors
 * @angles: array of target angles
 */
void servos_set(uint quantity, servo* motors, float *angles) ;

/**
 * servos_smooth() - smooth move multiple servo motors to angles
 * @quantity: quantity of servo motors
 * @motors: array of servo motors
 * @angles: array of target angles
 */
void servos_smooth(uint quantity, servo* motors, float *angles) ;


#endif