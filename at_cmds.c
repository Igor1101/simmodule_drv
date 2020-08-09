#include "serial.h"
#include "stm32f4xx_hal.h"
#include "at_cmds.h"

void at_cmd(void)
{
	AT_CMD("AT");
	pr_debugln("AT RESPONSE:%s", serial_getdata(SERIAL_AT, 400));
}

void at_task_func(void const * argument)
{
	pr_debugln("AT TASK");
	at_cmd();
	while(1) {
		osDelay(100);
	}
}
