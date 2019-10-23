#include "function.h"

extern uint32_t e_Systick_ui32;
extern fifo_t e_BUFFER_ui8;

void Delay_ms(uint32_t ms)
{
	uint32_t currenttick = e_Systick_ui32;
	while((e_Systick_ui32 - currenttick) <= ms);
}


uint8_t get_buffer(uint8_t arr[])
{
	uint8_t k, ui8l_temp;
	if(fifo_is_empty(e_BUFFER_ui8) == 1)
            return 0;
    else
    {
    	uint8_t ui8l_temp,i;
    	for(k = 0; k < 4; k++)
        {
    		fifo_get(e_BUFFER_ui8, &ui8l_temp);
    		arr[k] = ui8l_temp;
        }
        return 1;
     }
}
