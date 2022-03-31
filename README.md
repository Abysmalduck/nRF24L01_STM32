# Driver for NRF24L01+

Lite driver(library) for STM32

## Code example
### Transmitter
main.cpp
```cpp
#include "nRF24_driver.h"
#include "spi.h"
#include "gpio.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_SPI1_Init();

    GPIO_PIN nrf1_ce = {GPIOx, GPIO_PIN_x};
    GPIO_PIN nrf1_csn = {GPIOx, GPIO_PIN_x};

    nRF24 nrf1(&hspi1, &nrf1_csn, &nrf1_ce);

    nrf1.init();                                                    //General module init (flush all buffers, IRQ etc.)
    nrf1.setup_crc(true, true);                                     //Enable CRC check, 2byte mode
    nrf1.setup_IRQ(false, false, false);                            //Disable RX IRQ, TX IRQ and MRT IRQ
    nrf1.setup_Addr_length(NRF24_SETUPAW_5bytes);                   //5 bytes address length
    nrf1.setup_auto_ack(NRF24_ACK_DELAY_250, NRF24_ACK_COUNT_15);   //Send acknowledge after 250us, 15 times

    nrf1.setup_DynamicPayload(0, false);                            //Disable dynamic payload in pipe 0
    nrf1.setup_rf(NRF24_SPEED_250kbps, NRF24_POWER_0dBm);           //Set 250 kbps speed, power 0dBm

    uint8_t addr[5] = {0x01, 0x23, 0x45, 0x67, 0x89};               //Address for pipe0

    nrf1.openRXpipe(addr, 5, 0, 4);                                 //Open RX pipe (for acknowledge)
    nrf1.openTXpipe(addr, 5);                                       //Open TX pipe 

    nrf1.transmitModeSwitch(5);                                     //Switch to TX mode

    char data[16] = "data to transmit";

    while (true)
    {
        nrf1.transmit(data, 15);                                     //transmit data every 500ms
        HAL_Delay(500);
    }
}
```

### Reciever (no IRQ)
main.cpp
```cpp
#include "nRF24_driver.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_SPI1_Init();

    GPIO_PIN nrf1_ce = {GPIOx, GPIO_PIN_x};
    GPIO_PIN nrf1_csn = {GPIOx, GPIO_PIN_x};

    nRF24 nrf1(&hspi1, &nrf1_csn, &nrf1_ce);

    nrf1.init();                                                    //General module init (flush all buffers, IRQ etc.)
    nrf1.setup_crc(true, true);                                     //Enable CRC check, 2byte mode
    nrf1.setup_IRQ(true, false, false);                             //Enable RX IRQ, Disable TX IRQ and MRT IRQ
    nrf1.setup_Addr_length(NRF24_ADDR_LEN_5bytes);                  //5 bytes address length
    nrf1.setup_auto_ack(NRF24_ACK_DELAY_250, NRF24_ACK_COUNT_15);   //Send acknowledge after 250us, 15 times

    nrf1.setup_DynamicPayload(0, false);                            //Disable dynamic payload in pipe 0
    nrf1.setup_rf(NRF24_SPEED_250kbps, NRF24_POWER_0dBm);           //Set 250 kbps speed, power 0dBm

    uint8_t addr[5] = {0x01, 0x23, 0x45, 0x67, 0x89};               //Address for pipe0

    nrf1.openRXpipe(addr, 5, 0, 4);                                 //Open RX pipe 
    nrf1.openTXpipe(addr, 5);                                       //Open TX pipe (for acknowledge)

    nrf1.receiveModeSwitch(5);                                      //Switch to RX mode

    char data[16] = {0};

    while (true)
    {
        if (nrf1.isDataAvalible())                                  //Check is data avalible
        {
            nrf1.receive(data, 16);                                 //Read receive buffer
        }
    }
}
```
