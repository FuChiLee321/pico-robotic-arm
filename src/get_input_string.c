#include <stdio.h>
#include "pico/stdlib.h"
#include "get_input_string.h"

/**
 * @param c Character to check
 * @return True if c is the end of a line
 */
bool is_end_of_line(char c) {
    switch (c) {
    case '\n':  case '\r':  case (char)EOF:
        return true;
    }
    return false;
}

/**
 * @param c Character to check
 * @return True if c is a space or the end of a line
 */
bool is_space(char c) {
    switch (c) {
    case ' ':   case '\t':  case '\n':  case '\r':  case (char)EOF:
        return true;
    }
    return false;
}

/**
 * Get string input and store it in the provided buffer. 
 * Blocks until space, newline, tab, or carriage return is received or the buffer is full.
 * 
 * @param buffer Buffer to store the input string
 * @param buffer_size Size of the buffer
 * @return Returns the number of characters read.
 */
int get_string(char* buffer, int buffer_size) {
    int len = 0;
    int input = getchar();
    do {
        buffer[len++] = input;
        input = getchar_timeout_us(0);
    } while(input != PICO_ERROR_TIMEOUT && !is_space(input) && len < buffer_size - 1);
    buffer[len] = '\0';
    return len;
}

/**
 * Get string input and store it in the provided buffer.
 * Return if space, newline, tab, or carriage return is received before the timeout, buffer is full or timeout occurs.
 * 
 * @param buffer Buffer to store the input string
 * @param buffer_size Size of the buffer
 * @param timeout_us Timeout in microseconds
 * @return If space, newline, tab, or carriage return is received before the timeout,
 *         returns the number of characters read.
 *         If the timeout occurs, returns -1 * number of characters read.
 */
int get_string_timeout_us(char* buffer, int buffer_size, uint32_t timeout_us) {
    int len = 0;
    uint64_t time_end = make_timeout_time_us(timeout_us);
    do {
        int input = getchar_timeout_us(0);
        if(input != PICO_ERROR_TIMEOUT && !is_space(input))
            buffer[len++] = input;
        else if(len)
            break;
        if(len == buffer_size - 1 || time_reached(time_end))
            break;
        sleep_us(1);
    } while(true);
    buffer[len] = '\0';
    return time_reached(time_end) ? -len : len;
}

/**
 * Get string input and store it in the provided buffer.
 * Blocks until newline or carriage return is received or the buffer is full.
 * 
 * @param buffer Buffer to store the input string
 * @param buffer_size Size of the buffer
 * @return Returns the number of characters read.
 */
int get_line(char* buffer, int buffer_size)  {
    int len = 0;
    int input = getchar();
    do {
        buffer[len++] = input;
        input = getchar_timeout_us(0);
    } while(input != PICO_ERROR_TIMEOUT && !is_end_of_line(input) && len < buffer_size - 1);
    buffer[len] = '\0';
    return len;
}

/**
 * Get string input and store it in the provided buffer.
 * Return if newline or carriage return is received before the timeout, buffer is full or timeout occurs.
 * 
 * @param buffer Buffer to store the input string
 * @param buffer_size Size of the buffer
 * @param timeout_us Timeout in microseconds
 * @return If newline or carriage return is received before the timeout, returns the number of characters read.
 *         If the timeout occurs, returns -1 * number of characters read.
 */
int get_line_timeout_us(char* buffer, int buffer_size, uint32_t timeout_us) {
    int len = 0;
    uint64_t time_end = make_timeout_time_us(timeout_us);
    do {
        int input = getchar_timeout_us(0);
        if(input != PICO_ERROR_TIMEOUT && !is_end_of_line(input))
            buffer[len++] = input;
        else if(len)
            break;
        if(len == buffer_size - 1 || time_reached(time_end))
            break;
        sleep_us(1);
    } while(!time_reached(time_end));
    buffer[len] = '\0';
    return time_reached(time_end) ? -len : len;
}

