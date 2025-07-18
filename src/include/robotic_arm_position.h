#ifndef ROBOTIC_ARM_POSITION_H
#define ROBOTIC_ARM_POSITION_H

#include "struct_robotic_arm.h"
#include "struct_position_required.h"
#include "struct_coordinate_system.h"

/**
 * Set position required for a robotic arm.
 * 
 * @param robot Robotic arm to set position required
 * @param offsets_height Offset height of the robotic arm
 * @param offsets_radius Offset radius of the robotic arm
 * @param servo_plane_angle Servo to control plane angle
 * @param servos_from_base Array of servos from ground to position
 * @param servos_from_base_size Size of servos_from_base array
 * @param servos_angles_horizontal Angles to set radius same as sum of arm_lengths and offsets_radius
 * @param servos_direction If true, arm move from horizontal(positive radius)
 *                    to vertical(positive height) when angle increases
 * @param arm_lengths Array of arm lengths from ground to position
 */
void robotic_arm_set_position_required(robotic_arm* robot, float offsets_height, float offsets_radius,
                                        uint8_t servo_plane_angle, uint8_t* servos_from_base,
                                        uint8_t servos_from_base_size, float* servos_angles_horizontal,
                                        bool* servos_direction, float* arm_lengths);

/**
 * Translate cylindrical coordinate point to robotic arm control signal.
 * 
 * @param signal Robotic arm control signal to set
 * @param position_required Position required to translate
 * @param point Cylindrical coordinates to translate
 */
void cylindrical_to_robotic_arm_signal(robotic_arm_signal* signal, position_required* position_required,
                                        cylindrical_point* point);


#endif // ROBOTIC_ARM_POSITION_H