#include "nRF24_driver.h"

nRF24::nRF24(SPI_HandleTypeDef* SPI_PORT, GPIO_PIN* pin_csn, GPIO_PIN* pin_ce)
{
	_SPI_PORT = SPI_PORT;
	_pin_csn = pin_csn;
	_pin_ce = pin_ce;

	HAL_GPIO_WritePin(_pin_ce->_gpio_type, _pin_ce->_gpio_num, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_SET);

	writeReg(NRF24_REG_CONFIG, 0X0A);

	writeReg(NRF24_REG_EN_AA, NRF24_ENAA_P0);
	writeReg(NRF24_REG_EN_RXADDR, NRF24_EN_RXADDR_EPX_P0);
	writeReg(NRF24_REG_SETUP_AW, NRF24_SETUPAW_5bytes);
	writeReg(NRF24_REG_SETUP_PETR, NRF24_SETUPPETR_ARD_1500 | NRF24_SETUPPETR_ARC_15);
	writeReg(NRF24_REG_RF_SETUP, NRF24_RFSETUP_PF_PWR_0 | NRF24_RFSETUP_RATE_1000);


	HAL_GPIO_WritePin(_pin_ce->_gpio_type, _pin_ce->_gpio_num, GPIO_PIN_SET);
}

HAL_StatusTypeDef nRF24::readReg(uint8_t addr, uint8_t* data)
{
	HAL_StatusTypeDef status;
	uint8_t buff[2];
	buff[0] = addr;
	buff[1] = (uint8_t)NRF24_CMD_NOP;

	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_RESET);

	status = HAL_SPI_Transmit(_SPI_PORT, buff, 2, 1000);
	if (status != HAL_OK) return status;

	status = HAL_SPI_Receive(_SPI_PORT, data, 1, 1000);

	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_SET);

	return HAL_OK;
}

HAL_StatusTypeDef nRF24::writeReg(uint8_t addr, uint8_t data)
{
	HAL_StatusTypeDef status;

	uint8_t buff[2];

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

void nRF24::transmitModeSwitch(uint8_t* address, uint8_t addr_size, uint8_t channel)
{
	uint8_t buff;

	HAL_GPIO_WritePin(_pin_ce->_gpio_type, _pin_ce->_gpio_num, GPIO_PIN_RESET);

	writeBuffer(NRF24_REG_TX_ADDR, address, addr_size);
	writeBuffer(NRF24_REG_RX_ADDR_P0, address, addr_size);
	writeReg(NRF24_REG_RF_CH, channel);
	writeReg(NRF24_REG_RX_PW_P0, 32);


	readReg(NRF24_REG_CONFIG, &buff);

	buff &= ~(NRF24_CONFIG_PRIM_RRX);

	buff |= NRF24_CONFIG_PWR_UP;

	buff &= 0x0F;

	writeReg(NRF24_REG_CONFIG, buff);

	flushRX();
	flushTX();

	HAL_GPIO_WritePin(_pin_ce->_gpio_type, _pin_ce->_gpio_num, GPIO_PIN_SET);
}

void nRF24::receiveModeSwitch(uint8_t* address, uint8_t addr_size, uint8_t channel)
{
	HAL_GPIO_WritePin(_pin_ce->_gpio_type, _pin_ce->_gpio_num, GPIO_PIN_RESET);
	writeBuffer(NRF24_REG_TX_ADDR, address, addr_size);
	writeBuffer(NRF24_REG_RX_ADDR_P0, address, addr_size);
	writeReg(NRF24_REG_RF_CH, channel);
	writeReg(NRF24_REG_RX_PW_P0, 32);
	writeReg(NRF24_REG_CONFIG, 0X0B);
	HAL_GPIO_WritePin(_pin_ce->_gpio_type, _pin_ce->_gpio_num, GPIO_PIN_SET);

}

void nRF24::receiveIT(uint8_t* data, uint8_t size)
{
	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_RESET);

	uint8_t cmd = 0;
	cmd = NRF24_CMD_R_RX_PAYLOAD;

	HAL_StatusTypeDef st1 = HAL_SPI_Transmit(_SPI_PORT, &cmd, 1, 1000);
	HAL_StatusTypeDef st2 = HAL_SPI_Receive(_SPI_PORT, data, size, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_SET);
}

bool isDataAvalible(uint8_t pipe)
{

}

void nRF24::transmit(uint8_t* data, uint8_t size)
{
	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_RESET);

	uint8_t cmd = 0;
	cmd = NRF24_CMD_W_TX_PAYLOAD;

	HAL_StatusTypeDef st1 = HAL_SPI_Transmit(_SPI_PORT, &cmd, 1, 1000);
	HAL_StatusTypeDef st2 = HAL_SPI_Transmit(_SPI_PORT, data, size, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_SET);
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
