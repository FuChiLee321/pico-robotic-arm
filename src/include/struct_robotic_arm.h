#ifndef STRUCT_ROBOTIC_ARM_H
#define STRUCT_ROBOTIC_ARM_H

#include "servo_control.h"
#include "struct_position_required.h"

/**
 * @number: Number of servos in robotic arm (uint8_t)
 * @servos: Servos in robotic arm (servo*)
 * @position_required: Optional, can be used for position calculations
 */
typedef struct robotic_arm {
    uint8_t number;
    servo* servos;
    position_required* position_required;
} robotic_arm;

/**
 * @number: Number of servos to move (uint8_t)
 * @indexes: Indexes of servos to move (uint8_t*)
 * @angles: Target angles (float*)
 */
typedef struct robotic_arm_signal {
    uint8_t number;
    uint8_t* indexes;
    float* angles;
} robotic_arm_signal;



#endif // STRUCT_ROBOTIC_ARM_H