/*
 * at_cmds.h
 *
 *  Created on: Aug 1, 2020
 *      Author: igor
 */

#ifndef AT_SIMMODULE_AT_CMDS_H_
#define AT_SIMMODULE_AT_CMDS_H_
#include "serial.h"

#define AT_CMD(...) serial_print(SERIAL_AT, __VA_ARGS__ "\r\n")


#endif /* AT_SIMMODULE_AT_CMDS_H_ */
