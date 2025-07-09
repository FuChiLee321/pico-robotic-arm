#include <stdio.h>
#include "pico/stdlib.h"
#include "robotic_arm.h"


/**
 * Create a robotic arm that have number of servos.
 * 
 * @number: Number of servos in robotic arm
 */
robotic_arm* new_robotic_arm(uint8_t number) {
    robotic_arm* robot = malloc(sizeof(robotic_arm));
    if(!robot) {
        fprintf(stderr, "Robotic arm malloc failed.\n");
        return NULL;
    }
    robot->number = number;
    robot->servos = malloc(number * sizeof(servo));
    if(!robot->servos)
        fprintf(stderr, "Robotic arm servos malloc failed.\n");
    return robot;
}

/**
 * Set GPIO pin of a robotic arm servo.
 * 
 * @robot: Robotic arm to set
 * @index: Index of servo in robotic arm to set
 * @pin: GPIO pin connected to the servo, must support hardware PWM
 */
void robotic_arm_set_servo_pin(robotic_arm* robot, uint8_t index, uint pin) {
    if(index >= robot->number) {
        fprintf(stderr, "Index out of range.\n");
        return ;
    }
    robot->servos[index].pin = pin;
}

/**
 * Set information of a robotic arm servo from source.
 * 
 * @robot: Robotic arm to set
 * @index: Index of servo in robotic arm to set
 * @source: Servo to copy information
 */
void robotic_arm_set_servo_info(robotic_arm* robot, uint8_t index, servo* source) {
    if(index >= robot->number) {
        fprintf(stderr, "Index out of range.\n");
        return ;
    }
    SERVO_INFO_COPY(&robot->servos[index], source);
}

/**
 * Set a robotic arm servo to angle, not smoothly.
 * 
 * @robot: Robotic arm to set
 * @index: Index of servo in robotic arm to set
 * @angle: Target angle
 */
void robotic_arm_set_servo_angle(robotic_arm* robot, uint8_t index, float angle) {
    if(index >= robot->number) {
        fprintf(stderr, "Index out of range.\n");
        return ;
    }
    servo_set(&robot->servos[index], angle);
}

/**
 * Start robotic arm.
 * Make sure all servos are properly set before calling this.
 * 
 * @robot: Robotic arm to start
 */
void robotic_arm_start(robotic_arm* robot) {
    servos_init(robot->number, &robot->servos);
}

/**
 * Smoothly move a robotic arm servo to angle.
 * 
 * @robot: Robotic arm to move
 * @index: Index of servo in robotic arm to move
 * @angle: Target angle
 */
void robotic_arm_move_servo(robotic_arm* robot, uint8_t index, float angle) {
    if(index >= robot->number) {
        fprintf(stderr, "Index out of range.\n");
        return ;
    }
    servo_smooth(&robot->servos[index], angle);
}

/**
 * Smoothly move multiple robotic arm servos to angles at once.
 * 
 * @robot: Robotic arm to move
 * @signal: Control signals
 */
void robotic_arm_move(robotic_arm* robot, robotic_arm_signal* signal) {
    //
    servo* action_servos[signal->number];
    PICK_SERVOS(action_servos, robot->servos, signal->indexes, signal->number);
    servos_smooth(signal->number, action_servos, signal->angles);
}

/**
 * Print index and angle of a robotic arm servo
 * 
 * @robot: Robotic arm to print
 * @index: Index of servo in robotic arm to print
 */
void robotic_arm_print_servo(robotic_arm* robot, uint8_t index) {
    if(index >= robot->number) {
        fprintf(stderr, "Index out of range.\n");
        return ;
    }
    printf("Robotic arm servo %d : %f degrees", index, robot->servos[index].angle);
}

/**
 * Print all indexes and angles of robotic arm servos
 * 
 * @robot: Robotic arm to print
 */
void robotic_arm_print(robotic_arm* robot) {
    for(uint8_t i = 0; i < robot->number; i++)
        robotic_arm_print_servo(robot, i);
}

/**
 * Free memory malloced by new_robotic_arm().
 * 
 * @robot: Robotic arm to free
 */
void free_robotic_arm(robotic_arm* robot) {
    free(robot->servos);
    free(robot);
}
