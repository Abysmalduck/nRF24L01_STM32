#include "nRF24_driver.h"

nRF24::nRF24(SPI_HandleTypeDef* SPI_PORT, GPIO_PIN* pin_csn, GPIO_PIN* pin_ce)
{
	_SPI_PORT = SPI_PORT;
	_pin_csn = pin_csn;
	_pin_ce = pin_ce;

	tick = HAL_RCC_GetSysClockFreq();

	HAL_GPIO_WritePin(_pin_ce->_gpio_type, _pin_ce->_gpio_num, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_SET);

	writeReg(NRF24_REG_EN_AA, 0);
	writeReg(NRF24_REG_EN_RXADDR, 0);

	writeReg(NRF24_REG_SETUP_AW, NRF24_SETUPAW_5bytes);
	writeReg(NRF24_REG_SETUP_PETR, NRF24_SETUPPETR_ARD_1500 | NRF24_SETUPPETR_ARC_15);
}

void nRF24::setup_crc(bool crc_en)
{
	uint8_t buff = 0x00;
	readReg(NRF24_REG_CONFIG, &buff);
	buff = (buff & 0xF3) | ((uint8_t)crc_en << 3);
	writeReg(NRF24_REG_CONFIG, buff);

	return;
}

void nRF24::setup_crc(bool crc_en, bool twoByteMode)
{
	uint8_t buff = 0x00;
	readReg(NRF24_REG_CONFIG, &buff);
	buff = (buff & 0xF3) | ((uint8_t)crc_en << 3) | ((uint8_t)twoByteMode << 2);
	writeReg(NRF24_REG_CONFIG, buff);

	return;
}

void nRF24::setup_IRQ(bool en_RXDR, bool en_TXDS, bool en_MAXRT)
{
	uint8_t buff = 0x00;
	readReg(NRF24_REG_CONFIG, &buff);
	buff = (buff & 0x1F) | (uint8_t)!en_RXDR << 6 | (uint8_t)!en_TXDS << 5 | (uint8_t)!en_MAXRT << 4;
	writeReg(NRF24_REG_CONFIG, buff);

	return;
}
void nRF24::setup_Addr_length(uint8_t lenght)
{
	if ((lenght != NRF24_SETUPAW_3bytes) || (lenght != NRF24_SETUPAW_4bytes) || (lenght != NRF24_SETUPAW_5bytes))
	{
		writeReg(NRF24_REG_SETUP_AW, lenght);
	}
	return;
}

void nRF24::setup_auto_ack(uint8_t ack_delay, uint8_t ack_count)
{
	uint8_t buff = ack_delay | ack_count;
	writeReg(NRF24_REG_SETUP_PETR, buff);

	return;
}

void nRF24::setup_rf(uint8_t data_rate, uint8_t power)
{
	uint8_t buff = 0x00;
	buff = data_rate | power;
	writeReg(0x06, buff);
}

void nRF24::setup_DynamicPayload(uint8_t pipe_num, bool isEnabled)
{
	uint8_t buff = 0x00;
	readReg(NRF24_REG_DYNPD, &buff);

	buff = buff & ~(1 << pipe_num);
	buff = buff | 1 << pipe_num;

	writeReg(NRF24_REG_DYNPD, buff);

	return;
}

inline void nRF24::microsecondsDelay(uint32_t delay)
{
	delay *= tick / 1000000;

	while(delay--);

}

HAL_StatusTypeDef nRF24::readReg(uint8_t addr, uint8_t* data)
{
	HAL_StatusTypeDef status;
	uint8_t cmd = 0xFF;;
	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_RESET);
	status = HAL_SPI_TransmitReceive(_SPI_PORT, &addr, data, 1, 1000);
	if (addr != NRF24_REG_STATUS)
	{
		status = HAL_SPI_TransmitReceive(_SPI_PORT, &cmd, data, 1, 1000);
	}
	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_SET);

	return status;
}

HAL_StatusTypeDef nRF24::writeReg(uint8_t addr, uint8_t data)
{
	HAL_StatusTypeDef status;

	uint8_t buff[2] = {0};

	buff[0] = addr | NRF24_CMD_W_REGISTER;
	buff[1] = data;

	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_RESET);

	status = HAL_SPI_Transmit(_SPI_PORT, buff, 2, 1000);

	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_SET);
	return status;
}

HAL_StatusTypeDef nRF24::readBuffer(uint8_t addr, uint8_t *buff, uint8_t size)
{
	HAL_StatusTypeDef status;
	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_RESET);

	status = HAL_SPI_Transmit(_SPI_PORT, &addr, 1, 1000);
	if (status != HAL_OK)
	{
		HAL_SPI_Transmit(_SPI_PORT, (uint8_t*)NRF24_CMD_NOP, size, 1000);
		HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_SET);
		return status;
	}
	status = HAL_SPI_Receive(_SPI_PORT, buff, size, 1000);

	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_SET);
	return status;
}

HAL_StatusTypeDef nRF24::writeBuffer(uint8_t addr, uint8_t *buff, uint8_t size)
{
	HAL_StatusTypeDef status;
	addr = addr | NRF24_CMD_W_REGISTER;
	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_RESET);

	HAL_SPI_Transmit(_SPI_PORT, &addr, 1, 1000);
	status = HAL_SPI_Transmit(_SPI_PORT, buff, size, 1000);

	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_SET);

	return status;
}

HAL_StatusTypeDef nRF24::flushRX(void)
{
	return sendCommand(NRF24_CMD_FLUSH_RX);
}

HAL_StatusTypeDef nRF24::flushTX(void)
{
	return sendCommand(NRF24_CMD_FLUSH_TX);
}

HAL_StatusTypeDef nRF24::sendCommand(uint8_t command)
{
	HAL_StatusTypeDef status;

	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_RESET);

	status = HAL_SPI_Transmit(_SPI_PORT, &command, 1, 100);

	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_SET);
	return status;
}


void nRF24::transmitModeSwitch(uint8_t channel)
{
	uint8_t buff = 0x00;

	readReg(NRF24_REG_CONFIG, &buff);
	buff = (buff & 0xFD) | NRF24_CONFIG_PWR_UP;
	writeReg(NRF24_REG_CONFIG, buff);
	writeReg(NRF24_REG_RF_CH, channel);

	flushRX();
	flushTX();

	microsecondsDelay(130);
}

void nRF24::receiveModeSwitch(uint8_t channel)
{
	uint8_t buff = 0x00;

	readReg(NRF24_REG_CONFIG, &buff);
	buff = (buff & 0xFC) | NRF24_CONFIG_PWR_UP | NRF24_CONFIG_PRIM_RRX;
	writeReg(NRF24_REG_CONFIG, buff);

	writeReg(NRF24_REG_RF_CH, channel);

	flushRX();
	flushTX();

	HAL_GPIO_WritePin(_pin_ce->_gpio_type, _pin_ce->_gpio_num, GPIO_PIN_SET);

	microsecondsDelay(130);
}

void nRF24::openTXpipe(uint8_t* address, uint8_t addr_size)
{
	writeBuffer(NRF24_REG_TX_ADDR, address, addr_size);

}
void nRF24::openRXpipe(uint8_t* address, uint8_t addr_size, uint8_t pipe_num, uint8_t payload_length)
{
	uint8_t buff;
	if (pipe_num > 5) return;

	uint8_t pipe_reg_addr = NRF24_REG_RX_ADDR_P0 + pipe_num;
	uint8_t pipe_pld_pen_addr = NRF24_REG_RX_PW_P0 + pipe_num;

	writeBuffer(pipe_reg_addr, address, addr_size);
	writeReg(pipe_pld_pen_addr, payload_length);


	readReg(NRF24_REG_EN_AA, &buff);
	buff = buff | 1 << pipe_num;
	writeReg(NRF24_REG_EN_AA, buff);

	readReg(NRF24_REG_EN_RXADDR, &buff);
	buff = buff | 1 << pipe_num;
	writeReg(NRF24_REG_EN_RXADDR, buff);

}


void nRF24::receiveIT(uint8_t* data, uint8_t size)
{
	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_RESET);

	uint8_t cmd = 0;
	cmd = NRF24_CMD_R_RX_PAYLOAD;

	HAL_SPI_Transmit(_SPI_PORT, &cmd, 1, 1000);
	HAL_SPI_Receive(_SPI_PORT, data, size, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_SET);

	writeReg(NRF24_REG_STATUS, 0x70);

	flushRX();
	flushTX();
}

bool nRF24::isDataAvalible()
{
	uint8_t buff = 0x00;
	readReg(NRF24_REG_STATUS, &buff);

	buff = buff & 0x0E;

	if (buff == 0b1110 || buff == 0b1100) return false;
	else return true;
}

tx_status nRF24::transmit(uint8_t* data, uint8_t size)
{
	flushRX();
	flushTX();

	uint8_t buff = 0x0;
	uint32_t start_time = HAL_GetTick();
	while (true)
	{
		readReg(NRF24_REG_STATUS, &buff);
		buff &= 0x01;
		if (buff == 0) break;
		if (HAL_GetTick() - start_time > 1000)
		{
			buff = 0x3;
			writeReg(NRF24_REG_STATUS, 0x70);
			return NRF24_TIMEOUT;
		}
	}
	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_RESET);

	uint8_t cmd = 0;
	cmd = NRF24_CMD_W_TX_PAYLOAD;

	HAL_SPI_Transmit(_SPI_PORT, &cmd, 1, 1000);
	HAL_SPI_Transmit(_SPI_PORT, data, size, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_SET);

	HAL_GPIO_WritePin(_pin_ce->_gpio_type, _pin_ce->_gpio_num, GPIO_PIN_SET);
	microsecondsDelay(20);
	HAL_GPIO_WritePin(_pin_ce->_gpio_type, _pin_ce->_gpio_num, GPIO_PIN_RESET);

	while(true)
	{
		readReg(NRF24_REG_STATUS, &buff);
		buff = buff >> 4;
		buff &= 0x3;
		if (buff > 0) break;
		if (HAL_GetTick() - start_time > 1000)
		{
			buff = 0x3;
			break;
		}
	}

	writeReg(NRF24_REG_STATUS, 0x70);
	return (tx_status)buff;
}

void nRF24::printRegisters(UART_HandleTypeDef* uart)
{
	HAL_UART_Transmit(uart, (uint8_t*)"\r\n", 2, 1000);

	char regNames[24][16] ={"CONF: ", "ENAA: ", "ENRXADDR: ", "SETUPAW: ", "SETUPPETR: ", "RFCH: ", "RFSETUP: ", "STATUS: ", "OBSERVETX: ", "RPD: ",
							"RXADDRP0: ", "RXADDRP1: ", "RXADDRP2: ", "RXADDRP3: ", "RXADDRP4: ", "RXADDRP5: ", "TX_ADDR: ", "RX_PW_P0: ", "RX_PW_P1: ",
							"RX_PW_P2: ", "RX_PW_P3: ", "RX_PW_P4: ", "RX_PW_P5: ", "FIFOSTAT: "};

	for (uint i = 0; i < 24; i++)
	{
		printReg(uart, i, regNames[i]);
	}

	printReg(uart, 0x1C, (char*)"DYNPD: ");
	printReg(uart, 0x1D, (char*)"FEATURE: ");
}

void nRF24::printReg(UART_HandleTypeDef* uart, uint8_t addr, char* name)
{

	char uint_char_buff[4];
	HAL_UART_Transmit(uart, (uint8_t*)name, strlen(name), 1000);

	if ((addr != 0x0A) & (addr != 0x0B) & (addr != 0x10))
	{
		uint8_t uint_buff = 0;
		readReg(addr, &uint_buff);
		itoa(uint_buff, uint_char_buff, 16);
		HAL_UART_Transmit(uart, (uint8_t*)"0x", 2, 1000);
		HAL_UART_Transmit(uart, (uint8_t*)uint_char_buff, strlen(uint_char_buff), 1000);
	}
	else
	{
		uint8_t uint_buff[5];
		readBuffer(addr, uint_buff, 5);

		for (uint i = 0; i < 5; i++)
		{
			itoa(uint_buff[i], uint_char_buff, 16);
			HAL_UART_Transmit(uart, (uint8_t*)"0x", 2, 1000);
			HAL_UART_Transmit(uart, (uint8_t*)uint_char_buff, strlen(uint_char_buff), 1000);
			HAL_UART_Transmit(uart, (uint8_t*)" ", 1, 1000);
		}
	}
	HAL_UART_Transmit(uart, (uint8_t*)"\r\n", 2, 1000);
}
