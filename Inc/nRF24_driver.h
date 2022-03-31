#ifndef SRC_NRF24_DRIVER_H_
#define SRC_NRF24_DRIVER_H_

#include "main.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//Registers
#define NRF24_REG_CONFIG			(uint8_t)0x00
#define NRF24_REG_EN_AA				(uint8_t)0x01
#define NRF24_REG_EN_RXADDR			(uint8_t)0x02
#define NRF24_REG_SETUP_AW			(uint8_t)0x03
#define NRF24_REG_SETUP_PETR		(uint8_t)0x04
#define NRF24_REG_RF_CH				(uint8_t)0x05
#define NRF24_REG_RF_SETUP			(uint8_t)0x06
#define NRF24_REG_STATUS			(uint8_t)0x07
#define NRF24_REG_RX_ADDR_P0		(uint8_t)0x0A
#define NRF24_REG_RX_ADDR_P1		(uint8_t)0x0B
#define NRF24_REG_TX_ADDR			(uint8_t)0x10
#define NRF24_REG_RX_PW_P0			(uint8_t)0x11
#define NRF24_REG_RX_PW_P1			(uint8_t)0x12
#define NRF24_REG_RX_PW_P2			(uint8_t)0x13
#define NRF24_REG_RX_PW_P3			(uint8_t)0x14
#define NRF24_REG_RX_PW_P4			(uint8_t)0x15
#define NRF24_REG_RX_PW_P5			(uint8_t)0x16
#define NRF24_REG_FIFO_STATUS		(uint8_t)0x17
#define NRF24_REG_DYNPD				(uint8_t)0x1C
#define NRF24_REG_FEATURE			(uint8_t)0x1D



//NRF24 Commands
#define NRF24_CMD_R_REGISTER		(uint8_t)0x00
#define NRF24_CMD_W_REGISTER		(uint8_t)0x20
#define NRF24_CMD_R_RX_PAYLOAD		(uint8_t)0x61
#define NRF24_CMD_W_TX_PAYLOAD		(uint8_t)0xA0
#define NRF24_CMD_FLUSH_TX			(uint8_t)0xE1
#define NRF24_CMD_FLUSH_RX			(uint8_t)0xE2
#define NRF24_CMD_REUSE_TX_PL		(uint8_t)0xE3
#define NRF24_CMD_R_RX_PL_WID		(uint8_t)0x60
#define NRF24_CMD_W_ACK_PAYLOAD		(uint8_t)0xA8
#define NRF24_CMD_W_TX_PLD_NO_ACK	(uint8_t)0xB0
#define NRF24_CMD_NOP				(uint8_t)0xFF

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

enum tx_status
{
	NRF24_UNKNOWN,
	NRF24_TX_MAX_RT,
	NRF24_TX_OK,
	NRF24_TIMEOUT
};

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

	uint32_t tick;

	inline void microsecondsDelay(uint32_t delay);

	HAL_StatusTypeDef readReg(uint8_t addr, uint8_t* data);
	HAL_StatusTypeDef writeReg(uint8_t addr, uint8_t data);

	HAL_StatusTypeDef writeBuffer(uint8_t addr, uint8_t *buff, uint8_t size);
	HAL_StatusTypeDef readBuffer(uint8_t addr, uint8_t *buff, uint8_t size);

	HAL_StatusTypeDef flushRX(void);
	HAL_StatusTypeDef flushTX(void);

	HAL_StatusTypeDef sendCommand(uint8_t command);

	void flushIRQ(void);
	void printReg(UART_HandleTypeDef* uart, uint8_t addr, char* name);

public:

	nRF24(SPI_HandleTypeDef* SPI_PORT, GPIO_PIN* pin_csn, GPIO_PIN* pin_ce);

	void setup_crc(bool crc_en);
	void setup_crc(bool crc_en, bool twoByteMode);
	void setup_IRQ(bool en_RXDR, bool en_TXDS, bool en_MAXRT);
	void setup_Addr_length(uint8_t lenght);
	void setup_auto_ack(uint8_t ack_delay, uint8_t ack_count);
	void setup_rf(uint8_t data_rate, uint8_t power);
	void setup_DynamicPayload(uint8_t pipe_num, bool isEnabled);

	void receiveModeSwitch(uint8_t channel);
	void transmitModeSwitch(uint8_t channel);
	void openTXpipe(uint8_t* address, uint8_t addr_size);
	void openRXpipe(uint8_t* address, uint8_t addr_size, uint8_t pipe_num, uint8_t payload_length);
	
	tx_status transmit(uint8_t* data, uint8_t size);
	void receive(uint8_t* data, uint8_t size);
	
	bool isDataAvalible();

	void printRegisters(UART_HandleTypeDef* uart);

};

#endif /* SRC_NRF24_DRIVER_H_ */
