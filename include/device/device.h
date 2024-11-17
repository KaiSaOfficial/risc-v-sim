#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "utils.h"

#define FLASH_Base 0x0800'0000u
#define FLASH_Size 0x0010'0000u

#define RAM_Base 0x1000'0000u
#define RAM_Size 0x0000'8000u

#define UART_Base 0x2000'0000u
#define UART_Size 0x0000'8000u

#define UART_CTRL (UART_Base + 0x0000'0000u)
#define UART_TX_DATA_BUF (UART_Base + 0x0000'00004u)
#define UART_RX_DATA_BUF (UART_Base + 0x0000'0000Cu)

#define GPIO_Base 0x3000'0000u
#define GPIO_Size 0x0000'8000u

#define OK 0
#define LoadAccessFault -1
#define StoreAMOAccessFault -2
#define IllegalInstruction -3

#endif //__DEVICE_H__
