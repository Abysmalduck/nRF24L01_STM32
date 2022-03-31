# Driver for NRF24L01+

Lite driver(library) for STM32

## Code example
### Transmitter
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

    nRF24 nrf1(&hspi1, nrf1_csn, nrf1_ce);

    nrf1.setup_crc(true);
    nrf1.setup_IRQ(true, false, false);
    nrf1.setup_Addr_length(NRF24_SETUPAW_5bytes);
    nrf1.setup_auto_ack(NRF24_SETUPPETR_ARD_250, NRF24_SETUPPETR_ARC_15);
    nrf1.setup_rf(NRF24_RFSETUP_RATE_250, NRF24_RFSETUP_PF_PWR_0);

    nrf1.transmitModeSwitch(5);

    uint8_t addr[5] = {0x01, 0x23, 0x45, 0x67, 0x89};

    nrf1.openTXpipe(addr, 5);
    nrf1.openRXpipe(addr, 5, 0, 16);       //RX pipe for ACK

    char data[16] = "data to transmit";

    while (true)
    {
        nrf1.transmit(data);
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

    nRF24 nrf1(&hspi1, nrf1_csn, nrf1_ce);

    nrf1.setup_crc(true);
    nrf1.setup_IRQ(true, false, false);
    nrf1.setup_Addr_length(NRF24_SETUPAW_5bytes);
    nrf1.setup_auto_ack(NRF24_SETUPPETR_ARD_250, NRF24_SETUPPETR_ARC_15);
    nrf1.setup_rf(NRF24_RFSETUP_RATE_250, NRF24_RFSETUP_PF_PWR_0);

    nrf1.receiveModeSwitch(5);

    uint8_t addr[5] = {0x01, 0x23, 0x45, 0x67, 0x89};

    nrf1.openTXpipe(addr, 5);           //TX pipe for ACK
    nrf1.openRXpipe(addr, 5, 0, 16);

    char data[16] = {0};

    while (true)
    {
        if (isDataAvalible)
        {
            nrf1.receive(data, 16);
        }
    }
}
```