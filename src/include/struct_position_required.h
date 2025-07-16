#ifndef STRUCT_POSITION_REQUIRED_H
#define STRUCT_POSITION_REQUIRED_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Struct to save data of a robotic arm to calculate position.
 * 
 * @offsets_height: Offset height of the robotic arm (float)
 * @offsets_radius: Offset radius of the robotic arm (float)
 * @servo_plane_angle: Servo to control plane angle (uint8_t)
 * @servos_from_base: Array of servos from ground to position (uint8_t*)
 * @servos_from_base_size: Size of servos_from_base array (uint8_t)
 * @servos_angles_horizontal: Angles to set radius same as sum of arm_lengths and offsets_radius (float*)
 * @servos_direction: If true, arm move from horizontal(positive radius) to vertical(positive height) when angle increases (bool*)
 * @arm_lengths: Array of arm lengths from ground to position (float*)
 */
typedef struct position_required {
    float offsets_height;
    float offsets_radius;
    uint8_t servo_plane_angle;
    uint8_t* servos_from_base;
    uint8_t servos_from_base_size;
    float* servos_angles_horizontal;
    bool* servos_direction;
    float* arm_lengths;
} position_required;

#endif // STRUCT_POSITION_REQUIRED_H