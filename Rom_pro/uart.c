#include "./inc/freestanding.h"
/*
 * The UART control registers are memory-mapped at address UART.
 * This macro returns the address of one of the registers.
 */
#define UART_REG_ADDRESS(reg) ((uint8_t *)(UART + reg))

/*
 * UART registers map
 */
#define UART_CTRL 0
#define UART_TX_DATA_BUF 4
#define UART_RX_DATA_BUF 8

#define uart_read_reg(reg) (*(UART_REG_ADDRESS(reg)))
#define uart_write_reg(reg, data) (*(UART_REG_ADDRESS(reg)) = (data))

void uart_init() {
    // init uart_ctrl reg to 0
    uart_write_reg(UART_CTRL, 0x00);
}

void uart_putc(char ch) {
    // fill send buf
    uart_write_reg(UART_TX_DATA_BUF, ch);
    // wait send over
    while ((uart_read_reg(UART_CTRL) & (1 << 1)) != (1 << 1)) {
    }
    // set TI to 0
    uart_write_reg(UART_CTRL, (uart_read_reg(UART_CTRL) & ~(1 << 1)));
}

void uart_puts(char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

char uart_getc() {
    // wait RI to 1
    while ((uart_read_reg(UART_CTRL) & (1 << 0)) != (1 << 0)) {
    }
    // set RI to 0
    uart_write_reg(UART_CTRL, (uart_read_reg(UART_CTRL) & ~(1 << 0)));
    // read receive buf
    return uart_read_reg(UART_RX_DATA_BUF);
}

void uart_gets(char *s, uint8_t len) {
    uint8_t i = 0;
    while (i < len - 1 && (*s = uart_getc()) != '\n') {
        ++i;
        ++s;
    }
    *(s + 1) = '\0';
}