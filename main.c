#include <stdio.h>
#include "adc.h"
#include "lcd.h"
#include "timer.h"
#include "imu.h"
#include "open_interface.h"

void imu_printChipInfo();
void imu_move_distance(float dis, oi_t *sensor_data, int speed);

int main() {
    lcd_init();
    imu_init();
    timer_init();
//    oi_t* sensorData = oi_alloc();
//    oi_init(sensorData);


    imu_writeReg(IMU_OPR_MODE, NDOF);
    imu_setDefaultUnits();

    //    imu_printChipInfo();


//    imu_move_distance(50, sensorData, 100);


    mag_t* mag;
    float heading;
    while(1){
        // 0 Degrees == North, 90 deg == east, +- 180 deg == south, -90 deg == west
        mag = imu_getMag();
        heading = mag->heading;
        if (heading >= -45 && heading < 45) {
            printf("North");
        } else if (heading >= 45 && heading < 135) {
            printf("East");
        } else if (heading >= -135 && heading < -45) {
            printf("West");
        } else {
            printf("South");
        }
        free(mag);
        fflush(stdout);

        timer_waitMillis(1000);
    }

}

void imu_move_distance(float dis, oi_t *sensor_data, int speed){
    if (!sensor_data) {
        return;
    }
    float v = 0, x = 0;
    int t = timer_getMicros();
    int tLast;
    acc_t* acc;
    int direction = (dis > 0) * 2 - 1;
    dis = abs(dis);


    oi_setWheels(speed * direction, speed * direction);
    tLast = t;
    while (x < dis) {
        acc = imu_getAcc();
        t = timer_getMicros();
        v = v + (acc->magnitude * (t/1000000.0 - tLast/1000000.0));
        x = x + (v * (t/10000.0 - tLast/10000.0));
        free(acc);
        tLast = t;
    }
    oi_setWheels(0, 0);
}

void imu_printChipInfo() {
    imu_info_t* chipInfo = imu_getChipInfo();

    lcd_printf("Chip ID: %d\n"
            "ACC ID: %d\n"
            "MAG ID: %d\n"
            "GYRO ID: %d\n",
            chipInfo->chipID,
            chipInfo->accID,
            chipInfo->magID,
            chipInfo->gyrID);

    timer_waitMillis(5000);

    lcd_printf("SW Rev: %d\n"
            "BL Rev: %d\n",
            chipInfo->swRevID,
            chipInfo->blRevID);


    timer_waitMillis(5000);
    lcd_clear();
}
