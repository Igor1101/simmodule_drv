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
#define SIM_CMD_PRINT(...) serial_println(SERIAL_AT, __VA_ARGS__)
#define SIM_CMD_RESP(INPUT, ...)				\
	do {								\
		sim_response_init();			\
		SIM_CMD_PRINT(__VA_ARGS__);			\
		HAL_Delay(SIM_RESPONSE_DELAY);	\
		memcpy(INPUT, recv_data, recv_data_p); 	\
		sim_response_deinit();			\
	}while(0)

#define SIM_CMD_DEBUG(...)				\
		memset(recv_data_buf, 0, sizeof recv_data_buf);\
		SIM_CMD_RESP(recv_data_buf,__VA_ARGS__);		\
		tty_println(__VA_ARGS__);		\
		tty_println("R:\"%s\"", recv_data_buf)

#define SIM_CMD(...)								\
		memset(recv_data_buf, 0, sizeof recv_data_buf);	\
		SIM_CMD_RESP(recv_data_buf,__VA_ARGS__)
#define SIM_OK						\
		if(sim_hasvalue(recv_data_buf,"OK"))
#define SIM_ERROR						\
		if(sim_hasvalue(recv_data_buf,"ERROR"))
#define SIM_HASVALUE(VALUE)				\
		if(sim_hasvalue(recv_data_buf,VALUE))
#define SIM_NOVALUE(VALUE)				\
		if(!sim_hasvalue(recv_data_buf,VALUE))

#define SIM_GETRESULT_BLK	serial_getdata(SERIAL_AT, 4000)

// special characters
#define CTRL_Z 26


extern char recv_data_buf[RECV_DATA_SZ];
bool sim_hasvalue(char*value, char*str);
void sim_receive_data(char data);
void sim_response_init(void);
void sim_response_deinit(void);
char* sim_cmd_resp(char*cmd);
bool sim_tcp_ip_con_init(void);

#endif /* AT_SIMMODULE_AT_CMDS_H_ */
