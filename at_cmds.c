#include "serial.h"


void at_cmd()
{
	serial_print(SERIAL_AT, "AT");
}

