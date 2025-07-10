#include <stdio.h>
#include "pico/stdlib.h"
#include "robotic_arm.h"


int main()
{
    stdio_init_all();

    while (!stdio_usb_connected()) {
        sleep_ms(100); // Wait for USB serial connection
    }

    sleep_ms(1000); // Wait for servos to initialize

    // Make servo information for MG996R servo
    servo mg996r;
    mg996r.min_duty = 500;
    mg996r.max_duty = 2500;
    mg996r.period = 20000; // 20ms period

    // Initialize the robotic arm
    robotic_arm* robot_arm = robotic_arm_create(6);
    if (!robot_arm) {
        fprintf(stderr, "Failed to create robotic arm.\n");
        return 1;
    }
    for(uint8_t i = 0; i < robot_arm->number; i++) {
        robotic_arm_setting_servo_pin(robot_arm, i, i + 16); // Set GPIO pins 16 to 21 for servos
        robotic_arm_setting_servo_info(robot_arm, i, &mg996r);
        robotic_arm_setting_servo_angle(robot_arm, i, 90.0f); // Initialize all servos to 90 degrees
        printf("Servo %d allocated at %p\n", i, (void*)&robot_arm->servos[i]);
    }
    printf("Robotic arm initialize with %d servos.\n", robot_arm->number);
    robotic_arm_start(robot_arm);
    printf("Robotic arm initialized with %d servos.\n", robot_arm->number);

    // Initialize control signal for robotic arm
    robotic_arm_signal control_signal;
    uint8_t control_servos[robot_arm->number];
    float target_angles[robot_arm->number];
    control_signal.indexes = control_servos;
    control_signal.angles = target_angles;
    control_signal.number = 0;
    printf("Control signal initialized.\n");

    char input[256];
    printf("Enter 'p' to print status\n or command (format: number index angle index angle ...) to move: ");

    while (true) {
        sleep_ms(100);  // Sleep to avoid busy waiting

        int command_start = getchar_timeout_us(0);
        if (command_start == PICO_ERROR_TIMEOUT) {
            continue; // No input, continue to next iteration
        }
        
        // Read the input command
        int i = 1;
        input[0] = command_start; // Store the first character
        while (i < sizeof(input) - 1) {
            int c = getchar_timeout_us(0);
            if (c == PICO_ERROR_TIMEOUT || c == '\n' || c == '\r') {
                break; // End of input or timeout
            }
            input[i++] = (char)c; // Store the character
        }
        input[i] = '\0'; // Null-terminate the string
        
        // Print robotic arm status if input is "status"
        if (input[0] == 'p' || input[0] == 'P') {
            robotic_arm_print(robot_arm);
            printf("Enter 'p' to print status\n or command (format: number index angle index angle ...) to move: ");
            continue;
        }
        if(input[0] == 't' || input[0] == 'T') {
            // Test command to move all servos to 60 degrees
            printf("Test command received, moving all servos to 60 degrees.\n");
            robotic_arm_move_by_string(robot_arm, "6 0 60 1 60 2 60 3 60 4 60 5 60");
            robotic_arm_print(robot_arm);
            printf("Enter 'p' to print status\n or command (format: number index angle index angle ...) to move: ");
            continue;
        }
        if(input[0] == 'r' || input[0] == 'R') {
            // Reset command to move all servos to 90 degrees
            printf("Reset command received, moving all servos to 90 degrees.\n");
            robotic_arm_move_by_string(robot_arm, "6 0 90 1 90 2 90 3 90 4 90 5 90");
            robotic_arm_print(robot_arm);
            printf("Enter 'p' to print status\n or command (format: number index angle index angle ...) to move: ");
            continue;
        }
        
        // Move robotic arm servos based on input
        robotic_arm_move_by_string(robot_arm, input);
        printf("Enter 'p' to print status\n or command (format: number index angle index angle ...) to move: ");
    }
}
