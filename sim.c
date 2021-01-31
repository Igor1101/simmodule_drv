#include <string.h>
#include <generic/serial.h>
#include <simmodule_drv/sim.h>
#include MCU_HEADER

static char recv_data[RECV_DATA_SZ] = {0};
static char parse_buf[RECV_DATA_SZ*PARSE_BUF_AMOUNT] = {0};
char recv_data_buf[RECV_DATA_SZ];

volatile static uint8_t recv_data_p = 0;
volatile static uint8_t parse_buf_p = 0;
volatile static bool recv_on = false;
volatile static bool parse_task_on = false;

static void clr_buf(void);
static void parse(void);

static void clr_buf(void)
{
	recv_data_p = 0;
	parse_buf_p = 0;
}

void sim_response_init(void)
{
	recv_data_p = 0;
	memset(recv_data, 0, sizeof recv_data);
	recv_on = true;
}

void sim_response_deinit(void)
{
	recv_on = false;
}



void sim_receive_data(char data)
{
	// here receive to parse buf
	if(data == '\n') {
		parse_task_on = true;
	}
	if(parse_buf_p >= sizeof parse_buf) {
		parse_task_on = true;
	} else {
		parse_buf[parse_buf_p++] = data;
	}
	// here receive to recv data
	if(recv_on) {
		if(recv_data_p < sizeof recv_data)
			recv_data[recv_data_p++] = data;
		else {
			// overflow signal
		}
	}
}

bool sim_hasvalue(char*str, char*value)
{
	if(strstr(str, value) != NULL)
		return true;
	return false;
}

static void parse(void)
{
	//tty_println("p:%s", parse_buf);
}

void at_task_func(void const * argument)
{
		SIM_CMD_DEBUG("ATE0");
		SIM_CMD_DEBUG("AT");
		SIM_CMD_DEBUG("ATE0");
		SIM_CMD_DEBUG("AT");
		SIM_CMD_DEBUG("AT+CSQ");
		SIM_CMD_DEBUG("ATI");
		SIM_CMD_DEBUG("AT+GSV");
		SIM_CMD_DEBUG("AT+CIMI");
		SIM_CMD_DEBUG("AT+GSN");
		SIM_CMD_DEBUG("AT+CGREG?");
		//AT_CMD_DEBUG("AT+CMGF=1");
		//AT_CMD_DEBUG("AT+CMGS=\"+phone\"");
		//serial_printf(SERIAL_AT, "\"qwerty%c", CTRL_Z);
		//tty_println("RES\"%s\" cmd:", AT_GETRESULT);
		//AT_CMD_DEBUG("ATD0966038461;");
}

bool sim_tcp_ip_con_init(void)
{
	// verify if SIM CARD is ready
	SIM_CMD_DEBUG("AT+CPIN?");
	SIM_NOVALUE("READY") {
		// not ready
		return false;
	}
	// verify NET registration
	SIM_CMD_DEBUG("AT+CREG?");
	SIM_NOVALUE("0,1") {
		// should be +CREG: 0,1
		return false;
	}
	// verify access to packet data transmission
	SIM_CMD_DEBUG("AT+CGATT?");
	SIM_NOVALUE("1") {
		// should be +CGATT: 1
		return false;
	}
	// set: use cmds to transmit data
	SIM_CMD_DEBUG("AT+CIPMODE=0");
	SIM_ERROR {
		return false;
	}
	// set: monosocket
	SIM_CMD_DEBUG("AT+CIPMUX=0");
	SIM_ERROR {
		return false;
	}
	return true;
}
