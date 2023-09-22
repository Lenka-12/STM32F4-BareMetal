#include "stm32f411xe.h"
#include "timebase.h"
#include "led.h"
int main(void)
{
		led_init();
		timebase_init();
         for (;;){
				 led_toggle();
				 delay(1000);
         
       
   	    }


}

