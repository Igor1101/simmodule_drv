/*
 * at_cmds.h
 *
 *  Created on: Aug 1, 2020
 *      Author: igor
 */

#ifndef AT_SIMMODULE_AT_CMDS_H_
#define AT_SIMMODULE_AT_CMDS_H_
#include <generic/serial.h>

#define AT_CMD(...) serial_println(SERIAL_AT, __VA_ARGS__)

#define AT_GETRESULT	serial_getdata(SERIAL_AT, 1000)

#define AT_CMD_DEBUG(...) do {\
	serial_println(SERIAL_AT, __VA_ARGS__); \
	pr_debugln("AT RESULT\"%s\"", AT_GETRESULT);\
	}while(0)


#endif /* AT_SIMMODULE_AT_CMDS_H_ */
