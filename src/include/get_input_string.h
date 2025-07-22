#ifndef GET_INPUT_STRING_H
#define GET_INPUT_STRING_H

#include <stdint.h>

/**
 * Get string input and store it in the provided buffer. 
 * Blocks until space, newline, tab, or carriage return is received or the buffer is full.
 * 
 * @param buffer Buffer to store the input string
 * @param buffer_size Size of the buffer
 * @return Returns the number of characters read.
 */
int get_string(char* buffer, int buffer_size);

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
int get_string_timeout_us(char* buffer, int buffer_size, uint32_t timeout_us);

/**
 * Get string input and store it in the provided buffer.
 * Blocks until newline or carriage return is received or the buffer is full.
 * 
 * @param buffer Buffer to store the input string
 * @param buffer_size Size of the buffer
 * @return Returns the number of characters read.
 */
int get_line(char* buffer, int buffer_size);

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
int get_line_timeout_us(char* buffer, int buffer_size, uint32_t timeout_us);



 #endif // GET_INPUT_STRING_H