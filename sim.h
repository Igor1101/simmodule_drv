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
#include "iwdg.h"

#define PARSE_BUF_AMOUNT 4
#define RECV_DATA_SZ 128
#define SIM_RESPONSE_DELAY 400
#define SIMC	SIM_CMD
#define SIM_CMD_PRINT(...) serial_println(SERIAL_AT, __VA_ARGS__)
#define SIM_CMD_RESP(INPUT, ...)				\
	do {								\
		sim_response_init();			\
		SIM_CMD_PRINT(__VA_ARGS__);				\
		HAL_Delay(SIM_RESPONSE_DELAY);	\
		__HAL_IWDG_RELOAD_COUNTER(&hiwdg);		\
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


extern volatile uint8_t recv_data_p;
extern volatile bool recv_on ;
extern char recv_data_buf[RECV_DATA_SZ];
extern char recv_data[RECV_DATA_SZ];
extern volatile bool sim_parse_task_on;
extern volatile uint8_t sim_parse_buf_p;
bool sim_hasvalue(char*value, char*str);
void sim_receive_data(int data);
void sim_response_init(void);
void sim_response_deinit(void);
char* sim_cmd_resp(char*cmd);
bool sim_tcp_con_init(void);
bool sim_tcp_con_deinit(void);
bool sim_tcp_open_con(void);
void sim_send_end(void);
bool sim_tcp_send(void*data, size_t sz);
void sim_task_parse(void);
// GPS control
bool sim_GPS_init(void);
bool sim_GPS_deinit(void);
void sim_GPS_startgetinfo(int times);
bool sim_GPS_corr_data(void);
#endif /* AT_SIMMODULE_AT_CMDS_H_ */
