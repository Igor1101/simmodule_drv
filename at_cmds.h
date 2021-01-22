/*
 * at_cmds.h
 *
 *  Created on: Aug 1, 2020
 *      Author: igor
 */

#ifndef AT_SIMMODULE_AT_CMDS_H_
#define AT_SIMMODULE_AT_CMDS_H_
#include <generic/serial.h>
#include <generic/display.h>
#include <string.h>

#define AT_CMD(...) serial_println(SERIAL_AT, __VA_ARGS__)

#define AT_GETRESULT	serial_getdata(SERIAL_AT, 4000)

#define AT_CMD_DEBUG(...) do {\
	serial_clear(); \
	serial_println(SERIAL_AT, __VA_ARGS__); \
	tty_println("RES\"%s\" cmd:", AT_GETRESULT);\
	tty_println(__VA_ARGS__);\
	} while(0)


#endif /* AT_SIMMODULE_AT_CMDS_H_ */
