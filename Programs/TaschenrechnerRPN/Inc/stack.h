 /**
  * @file stack.h 
  * @author
  * @date Oct 26
  * @brief This file is responsible for push and pop operations on a stack
  */

#define MAX_SIZE = 10

/**
 * @brief Pushes a number to the top of a stack
 * @param number the number to push
 * @return int identifier if an error occured
 */
int push(int number);

/**
 * @brief Pops a number from the top of the stack  
 * @param number the variable to pop the value to
 * @return int indentifier if an error occured
 */
int pop(int* number);
