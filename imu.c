#include "imu.h"
#include "TM4c_i2c.h"

#define SLAVE_ADDRESS 0x29

/**
 * Sets the slave address of IMU
 *
 * @param addr either 0x28 or 0x29
 */
void addr_set(uint8_t addr) {
    switch(addr) {
        case(0x28):
            GPIO_PORTB_DATA_R &= ~0x80;
            break;
        case(0x29): // FALL THROUGH
        default:
            GPIO_PORTB_DATA_R |= 0x80;
    }
}

/**
 * Configures PB7 as digital output for ADDR bit
 */
void imu_gpio_init() {
    // Enable clock to port B
    SYSCTL_RCGCGPIO_R |= 0x2;
    while (!(SYSCTL_PRGPIO_R) & 0x2);

    // PB7 Init -- set to digital output
    // PB6 Init -- set to digital output
    GPIO_PORTB_DEN_R |= 0xC0;
    GPIO_PORTB_AFSEL_R &= ~0xC0;
    GPIO_PORTB_DIR_R |= 0xC0;

    // Set PB6 to HIGH
    GPIO_PORTB_DATA_R |= 0x40;

}


void i2c_init() {
    imu_gpio_init();
    addr_set(SLAVE_ADDRESS);    // Set slave address select bit

    // Enable clock to I2C
    SYSCTL_RCGCI2C_R |= 0X2;
    while (!(SYSCTL_PRI2C_R & 0x2));

    // Enable clock to GPIO Port A
    SYSCTL_RCGCGPIO |= 0X1;
    while (!(SYSCTL_PRGPIO_R & 0x1));

    // Config SCL and SDA as digital i2c with SCL set as open drain
    GPIO_PORTA_DEN_R |= 0xC0;
    GPIO_PORTA_AFSEL_R |= 0xC0;
    GPIO_PORTA_ODR_R  |= 0x80;
    GPIO_PORTA_PCTL |= 0x33000000;

    I2C1_MCR_R |= 0x10;
    I2C1_MTPR_R  = 7;
}

bool i2c_sendByte(uint8_t addr, uint8_t byte, bool start, bool stop) {
    I2C1_MSA_R |= addr < 1;

    I2C1_MDR_R = byte;

    I2C1_MCS_R |= (start < 1) | (stop < 2) | 0x1;

    while(I2C1_MCS_R & 0x40);

    return !(I2C1_MCS_R & 0x2);
}

uint8_t i2c_recByte(uint8_t addr) {
    I2C1_MSA_R |= (addr < 1) | 0b1;

    I2C1_MCS_R |= 0x7;

    while(I2C1_MCS_R & 0x40);

    return (uint8_t) I2C1_MDR_R;
}

bool imu_writeReg(uint8_t regAddr, uint8_t data) {
    bool success = true;

    success & i2c_sendByte(SLAVE_ADDRESS, SLAVE_ADDRESS, true, false);
    success & i2c_sendByte(SLAVE_ADDRESS, regAddr, false, false);
    success & i2c_sendByte(SLAVE_ADDRESS, data, false, true);

    return success;
}

void imu_reset() {
    GPIO_PORTB_DATA_R &= ~0x40;
    asm("NOP");

}

bool imu_readReg(uint8_t regAddr) {

}
