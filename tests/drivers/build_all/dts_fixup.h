#if defined(CONFIG_HAS_DTS_I2C)

#ifndef CONFIG_ADT7420_NAME
#define CONFIG_ADT7420_NAME ""
#define CONFIG_ADT7420_I2C_ADDR 0
#define CONFIG_ADT7420_I2C_MASTER_DEV_NAME ""
#define CONFIG_ADT7420_GPIO_DEV_NAME ""
#define CONFIG_ADT7420_GPIO_PIN_NUM 0
#endif

#ifndef CONFIG_ADXL372_DEV_NAME
#define CONFIG_ADXL372_DEV_NAME ""
#define CONFIG_ADXL372_I2C_ADDR 0
#define CONFIG_ADXL372_I2C_MASTER_DEV_NAME ""
#define CONFIG_ADXL372_GPIO_DEV_NAME ""
#define CONFIG_ADXL372_GPIO_PIN_NUM 0
#endif

#ifndef CONFIG_APDS9960_DRV_NAME
#define CONFIG_APDS9960_DRV_NAME ""
#define CONFIG_APDS9960_I2C_DEV_NAME ""
#define CONFIG_APDS9960_GPIO_DEV_NAME ""
#define CONFIG_APDS9960_GPIO_PIN_NUM 0
#endif

#ifndef CONFIG_CCS811_NAME
#define CONFIG_CCS811_NAME ""
#define CONFIG_CCS811_I2C_MASTER_DEV_NAME ""
#define CONFIG_CCS811_I2C_ADDR 0
#endif

#ifndef CONFIG_FXAS21002_NAME
#define CONFIG_FXAS21002_NAME ""
#define CONFIG_FXAS21002_I2C_ADDRESS 0
#define CONFIG_FXAS21002_I2C_NAME ""
#define CONFIG_FXAS21002_GPIO_NAME ""
#define CONFIG_FXAS21002_GPIO_PIN 0
#endif

#ifndef CONFIG_FXOS8700_NAME
#define CONFIG_FXOS8700_NAME ""
#define CONFIG_FXOS8700_I2C_NAME ""
#define CONFIG_FXOS8700_I2C_ADDRESS 0
#define CONFIG_FXOS8700_GPIO_NAME ""
#define CONFIG_FXOS8700_GPIO_PIN 0
#endif

#ifndef CONFIG_HTS221_NAME
#define CONFIG_HTS221_NAME ""
#define CONFIG_HTS221_I2C_MASTER_DEV_NAME ""
#endif

#ifndef CONFIG_LIS3MDL_NAME
#define CONFIG_LIS3MDL_NAME ""
#define CONFIG_LIS3MDL_I2C_MASTER_DEV_NAME ""
#define CONFIG_LIS3MDL_I2C_ADDR 0x1e
#endif

#ifndef CONFIG_LPS25HB_DEV_NAME
#define CONFIG_LPS25HB_DEV_NAME ""
#define CONFIG_LPS25HB_I2C_ADDR 0
#define CONFIG_LPS25HB_I2C_MASTER_DEV_NAME ""
#endif

#ifndef CONFIG_LSM6DS0_DEV_NAME
#define CONFIG_LSM6DS0_DEV_NAME ""
#define CONFIG_LSM6DS0_I2C_ADDR 0
#define CONFIG_LSM6DS0_I2C_MASTER_DEV_NAME ""
#endif

#ifndef CONFIG_MAX30101_NAME
#define CONFIG_MAX30101_I2C_NAME ""
#define CONFIG_MAX30101_NAME ""
#endif

#ifndef CONFIG_MMA8451Q_NAME
#define CONFIG_MMA8451Q_NAME ""
#define CONFIG_MMA8451Q_I2C_NAME ""
#define CONFIG_MMA8451Q_I2C_ADDRESS 0
#endif

#ifndef CONFIG_GPIO_SX1509B_DEV_NAME
#define CONFIG_GPIO_SX1509B_DEV_NAME ""
#define CONFIG_GPIO_SX1509B_I2C_ADDR 0
#define CONFIG_GPIO_SX1509B_I2C_MASTER_DEV_NAME ""
#endif

#ifndef CONFIG_LSM6DSL_DEV_NAME
#define CONFIG_LSM6DSL_DEV_NAME ""
#define CONFIG_LSM6DSL_I2C_ADDR 0
#define CONFIG_LSM6DSL_I2C_MASTER_DEV_NAME ""
#define CONFIG_LSM6DSL_GPIO_DEV_NAME ""
#define CONFIG_LSM6DSL_GPIO_PIN_NUM 0
#endif

#ifndef CONFIG_LPS22HB_DEV_NAME
#define CONFIG_LPS22HB_DEV_NAME ""
#define CONFIG_LPS22HB_I2C_ADDR 0
#define CONFIG_LPS22HB_I2C_MASTER_DEV_NAME ""
#endif

#ifndef CONFIG_VL53L0X_NAME
#define CONFIG_VL53L0X_NAME ""
#define CONFIG_VL53L0X_I2C_ADDR 0
#define CONFIG_VL53L0X_I2C_MASTER_DEV_NAME ""
#endif

#ifndef CONFIG_LSM303DLHC_ACCEL_NAME
#define CONFIG_LSM303DLHC_ACCEL_NAME ""
#define CONFIG_LSM303DLHC_ACCEL_I2C_MASTER_DEV ""
#define CONFIG_LSM303DLHC_ACCEL_I2C_ADDR 0x19
#endif

#ifndef CONFIG_LSM303DLHC_MAGN_NAME
#define CONFIG_LSM303DLHC_MAGN_NAME ""
#define CONFIG_LSM303DLHC_MAGN_I2C_MASTER_DEV ""
#define CONFIG_LSM303DLHC_MAGN_I2C_ADDR 0x1e
#endif

#endif /* CONFIG_HAS_DTS_I2C */

#if defined(CONFIG_HAS_DTS_SPI)

#ifndef CONFIG_ADXL372_DEV_NAME
#define CONFIG_ADXL372_DEV_NAME ""
#define CONFIG_ADXL372_SPI_DEV_NAME ""
#define CONFIG_ADXL372_SPI_DEV_SLAVE 0
#define CONFIG_ADXL372_SPI_BUS_FREQ 8000000
#endif

#endif /* CONFIG_HAS_DTS_SPI */
