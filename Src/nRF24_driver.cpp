#include "nRF24_driver.h"

nRF24::nRF24(SPI_HandleTypeDef* SPI_PORT, GPIO_PIN* pin_csn, GPIO_PIN* pin_ce)
{
	_SPI_PORT = SPI_PORT;
	_pin_csn = pin_csn;
	_pin_ce = pin_ce;

	HAL_GPIO_WritePin(_pin_ce->_gpio_type, _pin_ce->_gpio_num, GPIO_PIN_SET);

	writeReg(NRF24_REG_CONFIG, NRF24_CONFIG_EN_CRC | NRF24_CONFIG_PWR_UP);
	HAL_Delay(5);

	writeReg(NRF24_REG_EN_AA, NRF24_ENAA_P1);
	writeReg(NRF24_REG_EN_RXADDR, NRF24_EN_RXADDR_EPX_P1);

	writeReg(NRF24_REG_SETUP_AW, NRF24_SETUPAW_3bytes);

	writeReg(NRF24_REG_SETUP_PETR, NRF24_SETUPPETR_ARD_1500 | NRF24_SETUPPETR_ARC_15);
	writeReg(NRF24_REG_FEATURE, 0x00);
	writeReg(NRF24_REG_DYNPD, 0x00);

	writeReg(NRF24_REG_STATUS, 0x70);

	writeReg(NRF24_REG_RF_CH, 76);
}

HAL_StatusTypeDef nRF24::readReg(uint8_t addr, uint8_t* data)
{
	HAL_StatusTypeDef status;

	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_RESET);

	HAL_SPI_Transmit(_SPI_PORT, &addr, 1, HAL_MAX_DELAY);

	HAL_SPI_Transmit(_SPI_PORT, (uint8_t*)NRF24_NOP, 1, HAL_MAX_DELAY);

	status = HAL_SPI_Receive(_SPI_PORT, data, 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(_pin_csn->_gpio_type,  _pin_csn->_gpio_num, GPIO_PIN_SET);

	return status;
}

HAL_StatusTypeDef nRF24::writeReg(uint8_t addr, uint8_t data)
{
	HAL_StatusTypeDef status;

	addr = addr | NRF24_W_REGISTER;

	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_RESET);

	HAL_SPI_Transmit(_SPI_PORT, &addr, 1, HAL_MAX_DELAY);

	status = HAL_SPI_Transmit(_SPI_PORT, &data, 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(_pin_csn->_gpio_type, _pin_csn->_gpio_num, GPIO_PIN_SET);

	return status;
}


