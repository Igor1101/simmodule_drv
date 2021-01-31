#include <generic/serial.h>
#include <simmodule_drv/sim.h>
#include MCU_HEADER

static char recv_data[RECV_DATA_SZ] = {0};
static char parse_buf[RECV_DATA_SZ*PARSE_BUF_AMOUNT] = {0};
char recv_data_dbg[RECV_DATA_SZ];

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

void sim_tcp_ip_con_init(void)
{

}
