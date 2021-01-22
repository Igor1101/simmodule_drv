#include <generic/serial.h>
#include MCU_HEADER
#include "at_cmds.h"

void at_cmd(void)
{
}

void at_task_func(void const * argument)
{

	pr_debugln("AT TASK");
	at_cmd();
	serial_println(SERIAL_AT, "");
		AT_CMD_DEBUG("ATE0");
		AT_CMD_DEBUG("AT");
		AT_CMD_DEBUG("ATE0");
		AT_CMD_DEBUG("AT");
		AT_CMD_DEBUG("AT+CSQ");
		AT_CMD_DEBUG("ATI");
		AT_CMD_DEBUG("AT+GSV");
		AT_CMD_DEBUG("AT+CIMI");
		AT_CMD_DEBUG("AT+GSN");
		AT_CMD_DEBUG("AT+CGREG?");
		AT_CMD_DEBUG("AT+CMGF=1");
		//AT_CMD_DEBUG("AT+CMGS=\"+fullnum\"");
#define CTRL_Z 26
		serial_printf(SERIAL_AT, "\"qwerty%c", CTRL_Z);
		tty_println("RES\"%s\" cmd:", AT_GETRESULT);
}
