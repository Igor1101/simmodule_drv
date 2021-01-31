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

#define PARSE_BUF_AMOUNT 4
#define RECV_DATA_SZ 128
#define SIM_RESPONSE_DELAY 4000
#define SIM_CMD(...) serial_println(SERIAL_AT, __VA_ARGS__)
#define SIM_CMD_RESP(INPUT, ...)				\
	do {								\
		sim_response_init();			\
		SIM_CMD(__VA_ARGS__);			\
		HAL_Delay(SIM_RESPONSE_DELAY);	\
		memcpy(INPUT, recv_data, recv_data_p); 	\
		sim_response_deinit();			\
	}while(0)

#define SIM_CMD_DEBUG(...)				\
		memset(recv_data_dbg, 0, sizeof recv_data_dbg);\
		SIM_CMD_RESP(recv_data_dbg,__VA_ARGS__);		\
		tty_println(__VA_ARGS__);		\
		tty_println("R:\"%s\"", recv_data_dbg)

#define SIM_GETRESULT_BLK	serial_getdata(SERIAL_AT, 4000)

// special characters
#define CTRL_Z 26


extern char recv_data_dbg[RECV_DATA_SZ];
void sim_receive_data(char data);
void sim_response_init(void);
void sim_response_deinit(void);
char* sim_cmd_resp(char*cmd);

#endif /* AT_SIMMODULE_AT_CMDS_H_ */
