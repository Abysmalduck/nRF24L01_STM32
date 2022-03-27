#ifndef SRC_NRF24_DRIVER_H_
#define SRC_NRF24_DRIVER_H_

#include "main.h"

//Registers
#define NRF24_REG_CONFIG			0x00
#define NRF24_REG_EN_AA				0x01
#define NRF24_REG_EN_RXADDR			0x02
#define NRF24_REG_SETUP_AW			0x03
#define NRF24_REG_SETUP_PETR		0x04
#define NRF24_REG_RF_CH				0x05
#define NRF24_REG_RF_SETUP			0x06
#define NRF24_REG_STATUS			0x07
#define NRF24_REG_RX_ADDR_P0		0x0A
#define NRF24_REG_RX_ADDR_P1		0x0B
#define NRF24_REG_TX_ADDR			0x10
#define NRF24_REG_RX_PW_P0			0x11
#define NRF24_REG_RX_PW_P1			0x12
#define NRF24_REG_FIFO_STATUS		0x17
#define NRF24_REG_DYNPD				0x1C
#define NRF24_REG_FEATURE			0x1D

#define NRF24_NOP					0xFF

#define NRF24_W_REGISTER			0x20

//CONFIG (0x00) Register
#define NRF24_CONFIG_MASK_RX_DR		(uint8_t)(1 << 6)
#define NRF24_CONFIG_MASK_TX_DS		(uint8_t)(1 << 5)
#define NRF24_CONFIG_MASK_MAX_RT	(uint8_t)(1 << 4)
#define NRF24_CONFIG_EN_CRC			(uint8_t)(1 << 3)
#define NRF24_CONFIG_CRCO			(uint8_t)(1 << 2)
#define NRF24_CONFIG_PWR_UP			(uint8_t)(1 << 1)
#define NRF24_CONFIG_PRIM_RRX		(uint8_t)(1 << 0)

//EN_AA (0x01) Register
#define NRF24_ENAA_P5				(uint8_t)(1 << 5)
#define NRF24_ENAA_P4				(uint8_t)(1 << 4)
#define NRF24_ENAA_P3				(uint8_t)(1 << 3)
#define NRF24_ENAA_P2				(uint8_t)(1 << 2)
#define NRF24_ENAA_P1				(uint8_t)(1 << 1)
#define NRF24_ENAA_P0				(uint8_t)(1 << 0)

//EN_RXADDR (0x02) Register
#define NRF24_EN_RXADDR_EPX_P5		(uint8_t)(1 << 5)
#define NRF24_EN_RXADDR_EPX_P4		(uint8_t)(1 << 4)
#define NRF24_EN_RXADDR_EPX_P3		(uint8_t)(1 << 3)
#define NRF24_EN_RXADDR_EPX_P2		(uint8_t)(1 << 2)
#define NRF24_EN_RXADDR_EPX_P1		(uint8_t)(1 << 1)
#define NRF24_EN_RXADDR_EPX_P0		(uint8_t)(1 << 0)

//SETUP_AW (0x03) Register
#define NRF24_SETUPAW_Illegal		(uint8_t)(0b00 << 0)
#define NRF24_SETUPAW_3bytes		(uint8_t)(0b01 << 0)
#define NRF24_SETUPAW_4bytes		(uint8_t)(0b10 << 0)
#define NRF24_SETUPAW_5bytes		(uint8_t)(0b11 << 0)

//SETUP_PETR (0x04) Register
#define NRF24_SETUPPETR_ARD_250		(uint8_t)(0b0000 << 4)
#define NRF24_SETUPPETR_ARD_500		(uint8_t)(0b0001 << 4)
#define NRF24_SETUPPETR_ARD_750		(uint8_t)(0b0010 << 4)
#define NRF24_SETUPPETR_ARD_1000	(uint8_t)(0b0011 << 4)
#define NRF24_SETUPPETR_ARD_1250	(uint8_t)(0b0100 << 4)
#define NRF24_SETUPPETR_ARD_1500	(uint8_t)(0b0101 << 4)
#define NRF24_SETUPPETR_ARD_1750	(uint8_t)(0b0110 << 4)
#define NRF24_SETUPPETR_ARD_2000	(uint8_t)(0b0111 << 4)

#define NRF24_SETUPPETR_ARD_2250	(uint8_t)(0b1000 << 4)
#define NRF24_SETUPPETR_ARD_2500	(uint8_t)(0b1001 << 4)
#define NRF24_SETUPPETR_ARD_2750	(uint8_t)(0b1010 << 4)
#define NRF24_SETUPPETR_ARD_3000	(uint8_t)(0b1011 << 4)
#define NRF24_SETUPPETR_ARD_3250	(uint8_t)(0b1100 << 4)
#define NRF24_SETUPPETR_ARD_3500	(uint8_t)(0b1101 << 4)
#define NRF24_SETUPPETR_ARD_3750	(uint8_t)(0b1110 << 4)
#define NRF24_SETUPPETR_ARD_4000	(uint8_t)(0b1111 << 4)

#define NRF24_SETUPPETR_ARC_0		(uint8_t)(0b0000 << 0)
#define NRF24_SETUPPETR_ARC_1		(uint8_t)(0b0001 << 0)
#define NRF24_SETUPPETR_ARC_2		(uint8_t)(0b0010 << 0)
#define NRF24_SETUPPETR_ARC_3		(uint8_t)(0b0011 << 0)
#define NRF24_SETUPPETR_ARC_4		(uint8_t)(0b0100 << 0)
#define NRF24_SETUPPETR_ARC_5		(uint8_t)(0b0101 << 0)
#define NRF24_SETUPPETR_ARC_6		(uint8_t)(0b0110 << 0)
#define NRF24_SETUPPETR_ARC_7		(uint8_t)(0b0111 << 0)

#define NRF24_SETUPPETR_ARC_8		(uint8_t)(0b1000 << 0)
#define NRF24_SETUPPETR_ARC_9		(uint8_t)(0b1001 << 0)
#define NRF24_SETUPPETR_ARC_10		(uint8_t)(0b1010 << 0)
#define NRF24_SETUPPETR_ARC_11		(uint8_t)(0b1011 << 0)
#define NRF24_SETUPPETR_ARC_12		(uint8_t)(0b1100 << 0)
#define NRF24_SETUPPETR_ARC_13		(uint8_t)(0b1101 << 0)
#define NRF24_SETUPPETR_ARC_14		(uint8_t)(0b1110 << 0)
#define NRF24_SETUPPETR_ARC_15		(uint8_t)(0b1111 << 0)

//RF_SETUP (0x06) Register
#define NRF24_RFSETUP_CONT_WAVE		(uint8_t)(1 << 7)

#define NRF24_RFSETUP_RF_DR_LOW		(uint8_t)(1 << 5)
#define NRF24_RFSETUP_PLL_LOCK		(uint8_t)(1 << 4)
#define NRF24_RFSETUP_RF_DR_HIGH	(uint8_t)(1 << 3)

#define NRF24_RFSETUP_PF_PWR_m18	(uint8_t)(0b00 << 1)
#define NRF24_RFSETUP_PF_PWR_m12	(uint8_t)(0b01 << 1)
#define NRF24_RFSETUP_PF_PWR_m6		(uint8_t)(0b10 << 1)
#define NRF24_RFSETUP_PF_PWR_0		(uint8_t)(0b11 << 1)

#define NRF24_RFSETUP_RATE_250		(uint8_t)(1 << 5) | (uint8_t)(0 << 3)
#define NRF24_RFSETUP_RATE_1000		(uint8_t)(0 << 5) | (uint8_t)(0 << 3)
#define NRF24_RFSETUP_RATE_2000		(uint8_t)(0 << 5) | (uint8_t)(1 << 3)


//STATUS (0x07) Register
#define NRF24_STATUS_RX_DR			(uint8_t)(1 << 6)
#define NRF24_STATUS_TX_DS			(uint8_t)(1 << 5)
#define NRF24_STATUS_MAX_RT			(uint8_t)(1 << 4)

//FIFO_STATUS (0x17) Register
#define NRF24_TX_REUSE				(uint8_t)(1 << 6)

//Dynamic Payload (0x1C) Register
#define NRF24_DYNPD_DPL_P5			(uint8_t)(1 << 5)
#define NRF24_DYNPD_DPL_P4			(uint8_t)(1 << 4)
#define NRF24_DYNPD_DPL_P3			(uint8_t)(1 << 3)
#define NRF24_DYNPD_DPL_P2			(uint8_t)(1 << 2)
#define NRF24_DYNPD_DPL_P1			(uint8_t)(1 << 1)
#define NRF24_DYNPD_DPL_P0			(uint8_t)(1 << 0)

//Feature (0x1D) Rgister
#define NRF24_FEATURE_EN_DPL		(uint8_t)(1 << 2)
#define NRF24_FEATURE_EN_ACK_PAY	(uint8_t)(1 << 1)
#define NRF24_FEATURE_EN_DYN_ACK	(uint8_t)(1 << 0)


struct GPIO_PIN
{
	GPIO_TypeDef* _gpio_type;
	uint16_t _gpio_num;
};

class nRF24
{
private:
	SPI_HandleTypeDef* _SPI_PORT;

	GPIO_PIN* _pin_csn;
	GPIO_PIN* _pin_ce;

public:
	nRF24(SPI_HandleTypeDef* SPI_PORT, GPIO_PIN* pin_csn, GPIO_PIN* pin_ce);

	HAL_StatusTypeDef readReg(uint8_t addr, uint8_t* data);
	HAL_StatusTypeDef writeReg(uint8_t addr, uint8_t data);

	HAL_StatusTypeDef writeBuffer(uint8_t addr, uint8_t *buff, uint8_t size);
	HAL_StatusTypeDef readeBuffer(uint8_t addr, uint8_t *buff, uint8_t size);

};

#endif /* SRC_NRF24_DRIVER_H_ */
