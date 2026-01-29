#include "bmp.h"

#define BMP_ADDR (0x76UL)
#define BMP_DR (0xF7UL)
#define BMP_ID_R (0xD0UL)
#define BMP_ID_VAL (0x58UL)
#define BMP_CONF_R (0xF4UL)
#define CALIB_DR (0x88UL)

#define DATA_BUFF_LEN (6)
#define CALIB_BUFF_LEN (24)

#define T_OVERSMPL_1X (1UL << 5)
#define S_OVERSMPL_1X (1UL << 2)
#define PWR_MODE_NORMAL ((1UL << 1) | 1UL)
#define CONF_VAL (T_OVERSMPL_1X | S_OVERSMPL_1X | PWR_MODE_NORMAL)
#define CONF_SLEEP (0UL)

int f;
int adapter_nr = 1;
char filename[20];

int read_calib_data(struct calibration_data *calib_dt);

int bmp_init(struct calibration_data *calib_dt){
	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);

	f = open(filename, O_RDWR);
	if (f < 0){
		perror("Failed to open i2c bus\n");
		goto err;
	}

	if (ioctl(f, I2C_SLAVE, BMP_ADDR) < 0){
		perror("Failed to set slave address\n");
		goto err;
	}

	int ret;

	ret = i2c_smbus_read_byte_data(f, BMP_ID_R);
	if (ret != BMP_ID_VAL){
		perror("Wrong ID value\n");
		goto err;
	}

	i2c_smbus_write_byte_data(f, BMP_CONF_R, CONF_VAL);

	ret = read_calib_data(calib_dt);
	if (ret){
		goto err;
	}

	return 0;

	err:
		close(f);
		return 1;
}

void bmp_exit(void){
	close(f);
}

int read_calib_data(struct calibration_data *calib_dt){
	uint8_t calib_buff[CALIB_BUFF_LEN];
	int ret = i2c_smbus_read_i2c_block_data(f, CALIB_DR, CALIB_BUFF_LEN, calib_buff);

	if (ret < 0){
		perror("Failed to read calibration data: %d\n");
		return ret;
	}

	calib_dt->t1 = ((uint16_t)((calib_buff[1] << 8) | calib_buff[0]));
	calib_dt->t2 = ((int16_t)((calib_buff[3] << 8) | calib_buff[2]));
	calib_dt->t3 = ((int16_t)((calib_buff[5] << 8) | calib_buff[4]));
	calib_dt->p1 = ((uint16_t)((calib_buff[7] << 8) | calib_buff[6]));
	calib_dt->p2 = ((int16_t)((calib_buff[9] << 8) | calib_buff[8]));
	calib_dt->p3 = ((int16_t)((calib_buff[11] << 8) | calib_buff[10]));
	calib_dt->p4 = ((int16_t)((calib_buff[13] << 8) | calib_buff[12]));
	calib_dt->p5 = ((int16_t)((calib_buff[15] << 8) | calib_buff[14]));
	calib_dt->p6 = ((int16_t)((calib_buff[17] << 8) | calib_buff[16]));
	calib_dt->p7 = ((int16_t)((calib_buff[19] << 8) | calib_buff[18]));
	calib_dt->p8 = ((int16_t)((calib_buff[21] << 8) | calib_buff[20]));
	calib_dt->p9 = ((int16_t)((calib_buff[23] << 8) | calib_buff[22]));

	return 0;
}

int get_calibrated_data(uint8_t *raw_data, struct calibration_data *calib_dt, struct bmp_sensdt *sensdt){
	int64_t var1, var2, tfine, p;

	const long pres_raw = (long)((raw_data[0] << 12) | (raw_data[1] << 4) | (raw_data[2] >> 4));
	const long temp_raw = (long)((raw_data[3] << 12) | (raw_data[4] << 4) | (raw_data[5] >> 4));

	var1 = ((((temp_raw >> 3) - ((int32_t)calib_dt->t1 << 1))) * ((int32_t)calib_dt->t2)) >> 11;
	var2 = (((((temp_raw >> 4) - ((int32_t)calib_dt->t1)) * ((temp_raw >> 4) - ((int32_t)calib_dt->t1))) >> 12) * \
		((int32_t)calib_dt->t3)) >> 14;
	tfine = var1 + var2;
	sensdt->temperature = (tfine * 5 + 128) >> 8;

	var1 = ((int64_t)tfine) - 128000;
	var2 = var1 * var1 * (int64_t)calib_dt->p6;
	var2 = var2 + ((var1 * (int64_t)calib_dt->p5) << 17);
	var2 = var2 + (((int64_t)calib_dt->p4) << 35);
	var1 = ((var1 * var1 * (int64_t)calib_dt->p3) >> 8) + ((var1 * (int64_t)calib_dt->p2) << 12);
	if (var1 == 0){
		return 0;
	}
	p = 1048576 - pres_raw;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = (((int64_t)calib_dt->p9) * (p >> 13)) >> 25;
	var2 = (((int64_t)calib_dt->p8) * p) >> 19;
	sensdt->pressure = ((p + var1 + var2) >> 8) + (((int64_t)calib_dt->p7) << 4);

	return 0;
}

int bmp_read_data(struct bmp_sensdt *sensdt, struct calibration_data *calib_dt){
	uint8_t data_buff[DATA_BUFF_LEN];
	int ret;

	ret = i2c_smbus_read_i2c_block_data(f, BMP_DR, DATA_BUFF_LEN, data_buff);
	if (ret < 0){
		perror("Failed to read sensor data\n");
		return 1;
	}

	ret = get_calibrated_data(data_buff, calib_dt, sensdt);
	if (ret){
		perror("Failed to calibrate data\n");
		return 1;
	}

	return 0;
}

