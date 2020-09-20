#include <generic/serial.h>
#include "stm32f4xx_hal.h"
#include "at_cmds.h"

void at_cmd(void)
{
}

void at_task_func(void const * argument)
{
	pr_debugln("AT TASK");
	at_cmd();
	while(1) {
		AT_CMD_DEBUG("AT");
		AT_CMD_DEBUG("AT+CSQ");
		AT_CMD_DEBUG("ATI");
		AT_CMD_DEBUG("AT+GSV");
		AT_CMD_DEBUG("AT+CIMI");
		AT_CMD_DEBUG("AT+GSN");
		AT_CMD_DEBUG("AT+CGREG?");
		//AT_CMD_DEBUG("ATD0966038461;");
	}
}
