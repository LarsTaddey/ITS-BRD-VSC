/**
 * @brief All operations take their operands from the stack and push their result to the stack.
 * THey return one of ERROR_* from main.h
 * 
 */

/**
 * @brief Adds two numbers
 * 
 */
int add();

/**
 * @brief Subtracts two numbers
 */
int sub();

/**
 * @brief Multiplies two numbers
 */
int mult();

/**
 * @brief Divides a number with another number
 * Results are rounded to a decimal
 */
int div();

/**
 * @brief Swaps the first to elements of the stack
 * 
 */
int swap();

/**
 * @brief Prints an amount of elements from the top of the stack
 * 
 * @param count how many elements to print from the stack
 */
int print_n(int count);

/**
 * @brief Clears the stack 
 */
void clear();
