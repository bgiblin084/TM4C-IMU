/**
 * Driver for BNO055 IMU.
 *
 * @author Braedon Giblin <bigblin@iastate.edu>
 */

#ifndef IMU_H_
#define IMU_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <inc/tm4c123gh6pm.h>


/** BNO055 Address A **/
#define BNO055_ADDRESS_A (0x28)
/** BNO055 Address B **/
#define BNO055_ADDRESS_B (0x29)
/** BNO055 ID **/
#define BNO055_ID (0xA0)



// Start Page 0 Register defines
#define IMU_CHIP_ID         0x00
#define IMU_ACC_ID          0x01
#define IMU_MAG_ID          0x02
#define IMU_GYR_ID_R        0x03
#define IMU_SW_REV_MSB      0x04
#define IMU_SW_REV_LSB      0x05
#define IMU_BL_VER_R        0x06



void imu_reset();
void imu_writeReg(uint8_t regAddr, uint8_t val);
uint8_t imu_readRegByte(uint8_t regAddr);
uint8_t* imu_readRegBytes(uint8_t regAddr, size_t dataLen);



#endif /* IMU_H_ */
