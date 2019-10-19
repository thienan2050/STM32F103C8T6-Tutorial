/*
 * debug.c
 *
 *  Created on: Oct 8, 2019
 *      Author: thien
 */
#include "debug.h"

/**
  * @brief  Display the binary value
  * @param  value: The value needed to be displayed.
  * @retval None
  */
void printBinary(int value)
{
	int counter, temp, space = 0;
	for (counter = 31; counter >= 0; counter--)
	{
		if(space == 4)
		{
			printf(" ");
			space = 0;
		}
		space++;
		temp = value >> counter;
		if (temp & 1)
			printf("1");
		else
			printf("0");
	}
	printf("\n");
}



