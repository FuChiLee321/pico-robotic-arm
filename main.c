#include <stdio.h>
#include "pico/stdlib.h"
#include "robotic_arm.h"
#include "string.h"


/**
 * Get input and transform to number.
 * returns -1 if input is 'q' or 'Q' to indicate exit, -2 for invalid input.
 */
int get_input_uint() {
    int number = 0;
    int input;
    // Skip whitespace characters
    do {
        input = getchar_timeout_us(0);
        sleep_ms(10); // Sleep to avoid busy waiting
    } while (input == PICO_ERROR_TIMEOUT || input == ' ' || input == '\n' || input == '\t');
    if (input >= '0' && input <= '9')
        number = input - '0';
    else if (input == 'q' || input == 'Q')
        return -1;  // If input is 'q' or 'Q', return -1 to indicate exit
    else
        return -2;  // Invalid input, return -2
    while (true) {
        input = getchar_timeout_us(0);
        if (input >= '0' && input <= '9')
            number = number * 10 + (input - '0');
        else
            break; // Break on non-digit input
    }
    return number;
}

/**
 * Get input and transform to float.
 * Returns -1.0f if input is 'q' or 'Q' to indicate exit, -2.0f for invalid input.
 */
float get_input_float() {
    float number = 0.0f;
    int input;
    // Skip whitespace characters
    do {
        input = getchar_timeout_us(0);
    } while (input == ' ' || input == '\n' || input == '\t');
    if (input >= '0' && input <= '9')
        number = input - '0';
    else if (input == 'q' || input == 'Q')
        return -1.0f;  // If input is 'q' or 'Q', return -1.0f to indicate exit
    else
        return -2.0f;  // Invalid input, return -2.0f
    while (true) {
        input = getchar_timeout_us(0);
        if (input >= '0' && input <= '9')
            number = number * 10 + (input - '0');
        else if (input == '.')
            break; // Break on decimal point
    }
    if (input != '.')
        return number; // If no decimal point, return the integer part
    float decimal_place = 0.1f;
    while (true) {
        input = getchar_timeout_us(0);
        if (input >= '0' && input <= '9') {
            number += (input - '0') * decimal_place;
            decimal_place *= 0.1f;
        } else
            break;
    }
    return number;
}


/**
 * Starter for the created robotic arm using only one type of servo motor.
 * 
 * @robot_arm: Pointer to the robotic arm structure, should be created with robotic_arm_create.
 * @motor: Pointer to the servo motor structure to be used for all servos in the robotic arm.
 */
void robotic_arm_starter(robotic_arm* robot_arm, servo* motor) {
    if (!robot_arm || !motor) {
        fprintf(stderr, "Invalid robotic arm or servo pointer.\n");
        return;
    }
    for (uint8_t i = 0; i < robot_arm->number; i++) {
        memcpy(&robot_arm->servos[i], motor, sizeof(servo));
        robotic_arm_set_servo_pin(robot_arm, i, i + 16); // Assuming GPIO pins 16 to 21 for servos
    }
    robotic_arm_set_servo_limits(robot_arm, 1, 3.0f, 177.0f); // Set limits for servo 1
    robotic_arm_start(robot_arm);
}

/**
 * Single servo control mode for the robotic arm.
 * Allows user to control a single servo by selecting its index and adjusting its angle.
 * 
 * @robotic_arm: Pointer to the robotic arm structure.
 */
void robotic_arm_single_servo_mode(robotic_arm* robot_arm) {
    char select_tip[] = "Enter servo index (0 to %d) to control, or 'q' to exit: ";
    char angle_tip[] = "Enter 'i' to increase angle, 'd' to decrease angle, 'r' to reselect servo,\n"
                        "  '*' to multiply delta angle by 2, '/' to divide delta angle by 2,\n"
                        "  'p' to print current angles, or 'q' to exit: ";
    char show_delta[] = "Delta angle: %.2f\n";
    printf(select_tip, robot_arm->number - 1);
    while (true) {
        int input = get_input_uint();
        uint8_t index = 0;
        float angle = 0;
        if(input == -1) {
            printf("Exiting single servo mode.\n");
            return; // Exit on 'q' or 'Q'
        } else if(input >= 0 && input < robot_arm->number) {
            index = (uint8_t)input; // Valid servo index
        } else {
            printf("Invalid input. Please try again.\n");
            do { input = getchar_timeout_us(0); } while (input != PICO_ERROR_TIMEOUT); // Clear input buffer
            printf(select_tip, robot_arm->number - 1);
            continue; // Invalid input, prompt again
        }
        bool servo_selected = true;
        float delta_angle = 1.0f; // Default angle change step
        printf(show_delta, delta_angle);
        angle = robot_arm->servos[index].angle; // Get current angle of the servo
        printf(angle_tip);
        while (servo_selected) {
            int command = getchar_timeout_us(0);
            if (command == PICO_ERROR_TIMEOUT) {
                sleep_ms(10); // Sleep to avoid busy waiting
                continue; // No input, continue to next iteration
            }
            switch (command) {
            case 'i': // Increase angle
            case 'I':
                angle += delta_angle;
                if (angle > robot_arm->servos[index].angle_upper_bound) {
                    angle = robot_arm->servos[index].angle_upper_bound; // Clamp to upper bound
                }
                robotic_arm_move_servo(robot_arm, index, angle);
                printf("Angle increased to: %.2f\n", angle);
                break;
            case 'd': // Decrease angle
            case 'D':
                angle -= delta_angle;
                if (angle < robot_arm->servos[index].angle_lower_bound) {
                    angle = robot_arm->servos[index].angle_lower_bound; // Clamp to lower bound
                }
                robotic_arm_move_servo(robot_arm, index, angle);
                printf("Angle decreased to: %.2f\n", angle);
                break;
            case 'p': // Print current angle
            case 'P':
                robotic_arm_print(robot_arm);
                printf("Current selected servo: %d\n", index);
                printf(show_delta, delta_angle);
                break;
            case '*': // Multiply delta angle by 2
                delta_angle *= 2.0f;
                printf("Delta angle multiplied to: %.2f\n", delta_angle);
                break;
            case '/': // Divide delta angle by 2
                delta_angle /= 2.0f;
                printf("Delta angle divided to: %.2f\n", delta_angle);
                break;
            case 'r': // Reselect servo
            case 'R':
                servo_selected = false; // Exit the inner loop to reselect servo
                printf(select_tip, robot_arm->number - 1);
                break;
            case 'q': // Exit single servo mode
            case 'Q':
                printf("Exiting single servo mode.\n");
                return;
            default:
                printf("Invalid command.\n");
                printf(angle_tip); // Prompt again for valid command
            }
            sleep_ms(10);
        }
    }
    
}

/**
 * Multiple servo control mode for the robotic arm.
 * Allows user to control multiple servos by specifying their indexes and target angles.
 * 
 * @robot_arm: Pointer to the robotic arm structure.
 */
void robotic_arm_multiple_servo_mode(robotic_arm* robot_arm) {
    // Initialize control signal for robotic arm
    uint8_t control_servos[robot_arm->number];
    float target_angles[robot_arm->number];
    robotic_arm_signal control_signal = {
        .indexes = control_servos,
        .angles = target_angles,
        .number = 0
    };
    char command_tip[] = "Enter command format: 'number index angle index angle ...',\n"
                         "where 'number' is the number of servos to control, 'index' is the servo index (0 to %d),\n"
                         "and 'angle' is the target angle for that servo. Enter 'q' to exit.\n";
    printf(command_tip, robot_arm->number - 1);
    while (true) {
        int input = get_input_uint();
        if (input == -1) {
            printf("Exiting multiple servo mode.\n");
            return; // Exit on 'q' or 'Q'
        } else if (input < 1 || input > robot_arm->number || input == -2) {
            printf("Invalid number of servos. Please enter a number between 1 and %d.\n", robot_arm->number);
            do { input = getchar_timeout_us(0); } while (input != PICO_ERROR_TIMEOUT); // Clear input buffer
            printf(command_tip, robot_arm->number - 1);
            continue; // Invalid input, prompt again
        }
        control_signal.number = (uint8_t)input; // Set number of servos to control
        for (uint8_t i = 0; i < control_signal.number; i++) {
            int index = get_input_uint();
            if (index < 0 || index >= robot_arm->number) {
                printf("Invalid servo index %d. Please enter an index between 0 and %d.\n", index, robot_arm->number - 1);
                do { index = getchar_timeout_us(0); } while (index != PICO_ERROR_TIMEOUT); // Clear input buffer
                printf(command_tip, robot_arm->number - 1);
                break; // Invalid index, prompt again
            }
            control_signal.indexes[i] = (uint8_t)index; // Store servo index
            float angle = get_input_float();
            if (angle < robot_arm->servos[index].angle_lower_bound) {
                angle = robot_arm->servos[index].angle_lower_bound; // Clamp to lower bound
            } else if (angle > robot_arm->servos[index].angle_upper_bound) {
                angle = robot_arm->servos[index].angle_upper_bound; // Clamp to upper bound
            }
            control_signal.angles[i] = angle; // Store target angle
        }
        // Move servos to target angles
        robotic_arm_move(robot_arm, &control_signal);
    }
}

/**
 * Costom control mode for the robotic arm.
 * 
 * @robot_arm: Pointer to the robotic arm structure.
 */
void robotic_arm_custom_control_mode(robotic_arm* robot_arm) {
    // Initialize control signal for robotic arm
    uint8_t control_servos[robot_arm->number];
    float target_angles[robot_arm->number];
    robotic_arm_signal control_signal = {
        .indexes = control_servos,
        .angles = target_angles,
        .number = 0
    };
    // Example custom action
    // These actions can be modified or extended as needed
    // String format: "number index angle index angle ..."
    char exam_action[][40] = {
        "3 0 60 1 60 2 60",
        "2 3 60 4 60",
        "1 5 120",
        "5 0 90 1 90 2 90 3 90 4 90",
        "3 0 120 1 60 2 60",
        "2 3 60 4 60",
        "1 5 90",
        "6 0 90 1 90 2 90 3 90 4 90 5 90"
    };
    char action_tip[] = "Enter 'a' to do exam_action, or 'q' to exit.\n";

    printf(action_tip);
    while (true) {
        int input = getchar_timeout_us(0);
        if (input == PICO_ERROR_TIMEOUT) {
            sleep_ms(10); // Sleep to avoid busy waiting
            continue; // No input, continue to next iteration
        }
        switch (input)
        {
        case 'a':
        case 'A':
            for(int i = 0; i < sizeof(exam_action) / sizeof(exam_action[0]); i++) {
                robotic_arm_signal_from_string(&control_signal, exam_action[i]);
                robotic_arm_move(robot_arm, &control_signal);
                sleep_ms(100);
            }
            break;
        case 'q':
        case 'Q':
            printf("Exiting custom control mode.\n");
            return; // Exit on 'q' or 'Q'
        default:
            printf("Invalid command. Please enter 'a' for action or 'q' to exit.\n");
            printf(action_tip); // Prompt again for valid command
            continue; // Continue to next iteration
        }
    }
}

int main()
{
    stdio_init_all();

    while (!stdio_usb_connected()) {
        sleep_ms(100); // Wait for USB serial connection
    }

    // Make servo information
    servo mg996r = {
        .angle_range = 180.0f,      // MG996R servo has a range of 180 degrees
        .period = 20000,            // PWM period in microseconds (20ms)
        .min_duty = 500,            // Minimum duty cycle for 0 degrees in microseconds
        .max_duty = 2500,           // Maximum duty cycle for 180 degrees in microseconds
        .angle = 90.0f,             // Initial angle set to 90 degrees
        .angle_lower_bound = 0.0f,  // Lower bound of angle
        .angle_upper_bound = 180.0f // Upper bound of angle
    };

    // Initialize the robotic arm that has 6 servos
    robotic_arm* robot_arm = robotic_arm_create(6);
    if (!robot_arm) {
        fprintf(stderr, "Failed to create robotic arm.\n");
        return 1;
    }
    robotic_arm_starter(robot_arm, &mg996r);
    printf("Robotic arm initialized with %d servos.\n", robot_arm->number);

    char mode_tip[] = "Enter 's' for single servo control, 'm' for multiple servos control,\n"
                        " 'c' for costom control, or 'p' to print current angles.\n";
    printf(mode_tip);

    while (true) {
        int command = getchar_timeout_us(0);
        if (command == PICO_ERROR_TIMEOUT) {
            sleep_ms(100);  // Sleep to avoid busy waiting
            continue;
        }

        switch (command) {
        // Single servo control commands
        case 's':
        case 'S':
            robotic_arm_single_servo_mode(robot_arm);
            break;
        // Multiple servo control commands
        case 'm':
        case 'M':
            robotic_arm_multiple_servo_mode(robot_arm);
            break;
        // Costom servo control commands
        case 'c':
        case 'C':
            robotic_arm_custom_control_mode(robot_arm);
            break;
        // Print current angles of all servos
        case 'p':
        case 'P':
            robotic_arm_print(robot_arm);
            break;
        default:
            printf("Invalid command. Please try again.\n");
        }
        printf(mode_tip); // Prompt again for valid command
    }
}
