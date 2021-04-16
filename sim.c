#include <string.h>
#include <generic/serial.h>
#include <generic/flash_mng.h>
#include <simmodule_drv/sim.h>
#include MCU_HEADER

char recv_data[RECV_DATA_SZ] = {0};
static char sim_parse_buf[RECV_DATA_SZ] = {0};
char recv_data_buf[RECV_DATA_SZ];
static char sim_GPS_buf[RECV_DATA_SZ];

volatile uint8_t recv_data_p = 0;
volatile uint8_t sim_parse_buf_p = 0;
volatile bool recv_on = false;
volatile bool sim_parse_task_on = false;

static void clr_buf(void);
static void parse(void);

static void clr_buf(void)
{
	recv_data_p = 0;
	sim_parse_buf_p = 0;
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

void sim_task_parse(void)
{
	// verify GPS part
	if(sim_GPS_corr_data()) {
		char*needed_part = strstr(sim_parse_buf, "+CGPSINFO");
		if(needed_part != NULL) {
			// find next \r\n occurence, change it to \0
			for(char*d = needed_part; *d != 0 && d < sim_parse_buf + sizeof sim_parse_buf;
					d++) {
				if(*d == '\n' || *d == '\r') {
					*d = '\0';
				}
			}
			// now copy GPS data to sim_GPS_buf
			strncpy(sim_GPS_buf, needed_part, sizeof sim_GPS_buf);
				}
	}

	memset(sim_parse_buf, 0, sizeof sim_parse_buf);
	sim_parse_buf_p = 0;
	sim_parse_task_on = false;
}

void sim_receive_data(int data)
{
	char cdata = (char)data;
	// here receive to parse buf
	if(sim_parse_task_on == false) {
		if(sim_parse_buf_p >= sizeof sim_parse_buf) {
			sim_parse_task_on = true;
		} else if((cdata == '\n' || cdata == '\r') && sim_parse_buf_p > 20) {
			sim_parse_buf[sim_parse_buf_p++] = cdata;
			sim_parse_task_on = true;
		} else {
			sim_parse_buf[sim_parse_buf_p++] = cdata;
		}
	}
	// here receive to recv data
	if(recv_on) {
		if(recv_data_p < sizeof recv_data)
			recv_data[recv_data_p++] = cdata;
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

bool sim_GPS_init(void)
{
	SIM_CMD_DEBUG("AT+CGPS=1");
	SIM_NOVALUE("OK") {
		return false;
	}
	return true;
}

bool sim_GPS_deinit(void)
{
	SIM_CMD_DEBUG("AT+CGPS=0");
	SIM_NOVALUE("OK") {
		return false;
	}
	return true;
}

bool sim_GPS_verify(void)
{
	SIMC("AT+CGPS?");
	SIM_NOVALUE("1,1") {
		return false;
	}
	return true;
}

void sim_GPS_startgetinfo(int times)
{
	SIM_CMD_DEBUG("AT+CGPSINFO=%d", times);
}

bool sim_GPS_corr_data(void)
{
	if(sim_hasvalue(sim_parse_buf, "+CGPSINFO")) {
		return true;
	}
	return false;
}

char* sim_GPS_get_data(void)
{
	if(sim_hasvalue(sim_GPS_buf, "+CGPSINFO")) {
		return sim_GPS_buf;
	}
	return NULL;
}
