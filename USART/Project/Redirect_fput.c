#include "stm32f10x_usart.h"
#include <stdio.h>

void delay(unsigned int nCount);

struct __FILE {
    int dummy;
};
/* You need this if you want use printf */
/* Struct FILE is implemented in stdio.h */
FILE __stdout;

int fputc(int ch, FILE *f) {
    /* Do your stuff here */
    /* Send your custom byte */
    /* Send byte to USART */
    USART_SendData(USART2, ch);
		/* If you do not delay this API, UART can only print the last character. I do not know the reason*/
    delay(1);
    /* If everything is OK, you have to return character written */
    return ch;
    /* If character is not correct, you can return EOF (-1) to stop writing */
    //return -1;
}


void delay(unsigned int nCount)
{
    unsigned int i, j;
 
    for (i = 0; i < nCount; i++)
        for (j = 0; j < 0x2AFF; j++);
}
