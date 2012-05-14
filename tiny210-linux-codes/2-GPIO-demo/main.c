#include "led.h"
#include "buzzer.h"
#include "button.h"
#include "lib.h"

int mymain(void)
{
	led_init();
	buzzer_init();
	button_init();
		
	while (1)
	{
		int num;
		int i;
		
		for (num = 1; num <= 4; num++)
		{	
			if (button_is_down(num))
			{
				for (i = 0; i < num; i++)
				{
					buzzer_on();
					led_on();
					delay();
					buzzer_off();
					led_off();
					delay();
				}
			}
		}
	}
	
	while (0)
	{
		if (button_is_down(1))
			buzzer_on();
		else
			buzzer_off();	
	}
	
	while (1)
	{
		led_on();
		buzzer_on();
		delay();
		
		led_off();
		buzzer_off();
		delay();
	}

	return 0;
}
