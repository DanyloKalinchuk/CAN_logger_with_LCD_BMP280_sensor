#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

struct calibration_data {
	uint16_t t1, p1;
	int16_t t2, t3, p2, p3, p4, p5, p6, p7, p8, p9;
};

struct bmp_sensdt {
	int temperature, pressure;
};

int bmp_init(struct calibration_data *calib_dt);
void bmp_exit(void);
int bmp_read_data(struct bmp_sensdt *sensdt, 
	struct calibration_data *calib_dt);

#endif

