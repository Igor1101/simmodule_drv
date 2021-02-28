#include <string.h>
#include <generic/serial.h>
#include <generic/flash_mng.h>
#include <simmodule_drv/sim.h>
#include MCU_HEADER

char recv_data[RECV_DATA_SZ] = {0};
static char parse_buf[RECV_DATA_SZ*PARSE_BUF_AMOUNT] = {0};
char recv_data_buf[RECV_DATA_SZ];

volatile uint8_t recv_data_p = 0;
volatile uint8_t parse_buf_p = 0;
volatile bool recv_on = false;
volatile bool parse_task_on = false;

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
		SIMC("ATE0");
		SIMC("AT");
		SIMC("ATE0");
		SIMC("AT");
		SIMC("AT+CSQ");
		SIMC("ATI");
		SIMC("AT+GSV");
		SIMC("AT+CIMI");
		SIMC("AT+GSN");
		SIMC("AT+CGREG?");
		//AT_CMD_DEBUG("AT+CMGF=1");
		//AT_CMD_DEBUG("AT+CMGS=\"+phone\"");
		//serial_printf(SERIAL_AT, "\"qwerty%c", CTRL_Z);
		//tty_println("RES\"%s\" cmd:", AT_GETRESULT);
		//AT_CMD_DEBUG("ATD0966038461;");
}

bool sim_tcp_con_init(void)
{
	// verify if SIM CARD is ready
	SIMC("AT+CPIN?");
	SIM_NOVALUE("READY") {
		// not ready
		return false;
	}
	// verify NET registration
	SIMC("AT+CREG?");
	SIM_NOVALUE("CREG") {
		// should be +CREG: 0,1
		return false;
	}
	//any previous connections should be closed
	SIMC("AT+NETCLOSE");
	SIMC("AT+CGSOCKCONT=1,\"IP\",\"CMNET\"");
	// PDP CONTEXT
	SIMC("AT+CGDCONT=1,\"IP\",\"CMNET\"");
	//Configure for non-transparent mode
	SIMC("AT+CIPMODE=0");
	// PDP profile number
	//Set the PDP profile number to use
	SIMC("AT+CSOCKSETPN=1");
	//Open the socket
	SIMC("AT+NETOPEN");
	//Get the IP address
	SIMC("AT+IPADDR");
	//Start the TCP connection
	SIMC("AT+CIPOPEN=0,\"TCP\",\"%s\",%s", fdata.server0_addr, fdata.conf_port);
	return true;
}

bool sim_tcp_send(void*data, size_t sz)
{
	SIMC("AT+CIPSEND=0,%d" , sz);
	sim_response_init();
	serial_write(SERIAL_AT, data, sz);
	sim_send_end();
	HAL_Delay(SIM_RESPONSE_DELAY);
	memcpy(recv_data_buf, recv_data, recv_data_p);
	tty_println("R:\"%s\"", recv_data_buf);
	sim_response_deinit();
	return true;
}

bool sim_tcp_con_deinit(void)
{
	SIMC("AT+CIPCLOSE=0");
	SIM_NOVALUE("OK") {
		return false;
	}
	return true;
}

void sim_send_end(void)
{
	char z[2] = { 0x03, CTRL_Z};
	serial_write(SERIAL_AT, z, sizeof z);
}
