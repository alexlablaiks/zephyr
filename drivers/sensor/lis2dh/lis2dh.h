/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_DRIVERS_SENSOR_LIS2DH_LIS2DH_H_
#define ZEPHYR_DRIVERS_SENSOR_LIS2DH_LIS2DH_H_

#include <kernel.h>
#include <device.h>
#include <sys/util.h>
#include <stdint.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <string.h>

#define LIS2DH_BUS_ADDRESS		DT_INST_0_ST_LIS2DH_BASE_ADDRESS
#define LIS2DH_BUS_DEV_NAME		DT_INST_0_ST_LIS2DH_BUS_NAME

#define LIS2DH_REG_WAI			0x0f
#define LIS2DH_CHIP_ID			0x33

#if defined(DT_ST_LIS2DH_BUS_SPI)
#include <drivers/spi.h>

#define LIS2DH_SPI_READ_BIT		BIT(7)
#define LIS2DH_SPI_AUTOINC		BIT(6)
#define LIS2DH_SPI_ADDR_MASK		BIT_MASK(6)

/* LIS2DH supports only SPI mode 0, word size 8 bits, MSB first */
#define LIS2DH_SPI_CFG			SPI_WORD_SET(8)

#elif defined(DT_ST_LIS2DH_BUS_I2C)
#include <drivers/i2c.h>
#else
#error "define bus type (I2C/SPI)"
#endif

#define LIS2DH_AUTOINCREMENT_ADDR	BIT(7)

#define LIS2DH_REG_CTRL0		0x1e
#define LIS2DH_SDO_PU_DISC_SHIFT	7
#define LIS2DH_SDO_PU_DISC_MASK		BIT(LIS2DH_SDO_PU_DISC_SHIFT)

#define LIS2DH_REG_CTRL1		0x20
#define LIS2DH_ACCEL_XYZ_SHIFT		0
#define LIS2DH_ACCEL_X_EN_BIT		BIT(0)
#define LIS2DH_ACCEL_Y_EN_BIT		BIT(1)
#define LIS2DH_ACCEL_Z_EN_BIT		BIT(2)
#define LIS2DH_ACCEL_EN_BITS		(LIS2DH_ACCEL_X_EN_BIT | \
					LIS2DH_ACCEL_Y_EN_BIT | \
					LIS2DH_ACCEL_Z_EN_BIT)
#define LIS2DH_ACCEL_XYZ_MASK		BIT_MASK(3)

#define LIS2DH_LP_EN_BIT_MASK		BIT(3)
#if defined(CONFIG_LIS2DH_OPER_MODE_LOW_POWER)
	#define LIS2DH_LP_EN_BIT	BIT(3)
#else
	#define LIS2DH_LP_EN_BIT	0
#endif

#define LIS2DH_ODR_1			1
#define LIS2DH_ODR_2			2
#define LIS2DH_ODR_3			3
#define LIS2DH_ODR_4			4
#define LIS2DH_ODR_5			5
#define LIS2DH_ODR_6			6
#define LIS2DH_ODR_7			7
#define LIS2DH_ODR_8			8
#define LIS2DH_ODR_9			9

#if defined(CONFIG_LIS2DH_ODR_1)
	#define LIS2DH_ODR_IDX		LIS2DH_ODR_1
#elif defined(CONFIG_LIS2DH_ODR_2)
	#define LIS2DH_ODR_IDX		LIS2DH_ODR_2
#elif defined(CONFIG_LIS2DH_ODR_3)
	#define LIS2DH_ODR_IDX		LIS2DH_ODR_3
#elif defined(CONFIG_LIS2DH_ODR_4) || defined(CONFIG_LIS2DH_ODR_RUNTIME)
	#define LIS2DH_ODR_IDX		LIS2DH_ODR_4
#elif defined(CONFIG_LIS2DH_ODR_5)
	#define LIS2DH_ODR_IDX		LIS2DH_ODR_5
#elif defined(CONFIG_LIS2DH_ODR_6)
	#define LIS2DH_ODR_IDX		LIS2DH_ODR_6
#elif defined(CONFIG_LIS2DH_ODR_7)
	#define LIS2DH_ODR_IDX		LIS2DH_ODR_7
#elif defined(CONFIG_LIS2DH_ODR_8)
	#define LIS2DH_ODR_IDX		LIS2DH_ODR_8
#elif defined(CONFIG_LIS2DH_ODR_9_NORMAL) || defined(CONFIG_LIS2DH_ODR_9_LOW)
	#define LIS2DH_ODR_IDX		LIS2DH_ODR_9
#endif

#define LIS2DH_ODR_SHIFT		4
#define LIS2DH_ODR_RATE(r)		((r) << LIS2DH_ODR_SHIFT)
#define LIS2DH_ODR_BITS			(LIS2DH_ODR_RATE(LIS2DH_ODR_IDX))
#define LIS2DH_ODR_MASK			(BIT_MASK(4) << LIS2DH_ODR_SHIFT)

#define LIS2DH_REG_CTRL2		0x21
#define LIS2DH_HPIS2_EN_BIT		BIT(1)
#define LIS2DH_FDS_EN_BIT		BIT(3)

#define LIS2DH_REG_CTRL3		0x22
#define LIS2DH_EN_DRDY1_INT1_SHIFT	4
#define LIS2DH_EN_DRDY1_INT1		BIT(LIS2DH_EN_DRDY1_INT1_SHIFT)

#define LIS2DH_REG_CTRL4		0x23
#define LIS2DH_FS_SHIFT			4
#define LIS2DH_FS_MASK			(BIT_MASK(2) << LIS2DH_FS_SHIFT)

#if defined(CONFIG_LIS2DH_ACCEL_RANGE_2G) ||\
	defined(CONFIG_LIS2DH_ACCEL_RANGE_RUNTIME)
	#define LIS2DH_FS_IDX		0
#elif defined(CONFIG_LIS2DH_ACCEL_RANGE_4G)
	#define LIS2DH_FS_IDX		1
#elif defined(CONFIG_LIS2DH_ACCEL_RANGE_8G)
	#define LIS2DH_FS_IDX		2
#elif defined(CONFIG_LIS2DH_ACCEL_RANGE_16G)
	#define LIS2DH_FS_IDX		3
#endif

#define LIS2DH_FS_SELECT(fs)		((fs) << LIS2DH_FS_SHIFT)
#define LIS2DH_FS_BITS			(LIS2DH_FS_SELECT(LIS2DH_FS_IDX))
#if defined(CONFIG_LIS2DH_OPER_MODE_HIGH_RES)
	#define LIS2DH_HR_BIT		BIT(3)
#else
	#define LIS2DH_HR_BIT		0
#endif

#define LIS2DH_REG_CTRL5		0x24
#define LIS2DH_LIR_INT2_SHIFT		1
#define LIS2DH_EN_LIR_INT2		BIT(LIS2DH_LIR_INT2_SHIFT)

#define LIS2DH_REG_CTRL6		0x25
#define LIS2DH_EN_INT2_INT2_SHIFT	5
#define LIS2DH_EN_INT2_INT2		BIT(LIS2DH_EN_INT2_INT2_SHIFT)

#define LIS2DH_REG_REFERENCE		0x26

#define LIS2DH_REG_STATUS		0x27
#define LIS2DH_STATUS_ZYZ_OVR		BIT(7)
#define LIS2DH_STATUS_Z_OVR		BIT(6)
#define LIS2DH_STATUS_Y_OVR		BIT(5)
#define LIS2DH_STATUS_X_OVR		BIT(4)
#define LIS2DH_STATUS_OVR_MASK		(BIT_MASK(4) << 4)
#define LIS2DH_STATUS_ZYX_DRDY		BIT(3)
#define LIS2DH_STATUS_Z_DRDY		BIT(2)
#define LIS2DH_STATUS_Y_DRDY		BIT(1)
#define LIS2DH_STATUS_X_DRDY		BIT(0)
#define LIS2DH_STATUS_DRDY_MASK		BIT_MASK(4)

#define LIS2DH_REG_ACCEL_X_LSB		0x28
#define LIS2DH_REG_ACCEL_Y_LSB		0x2A
#define LIS2DH_REG_ACCEL_Z_LSB		0x2C
#define LIS2DH_REG_ACCEL_X_MSB		0x29
#define LIS2DH_REG_ACCEL_Y_MSB		0x2B
#define LIS2DH_REG_ACCEL_Z_MSB		0x2D

#define LIS2DH_REG_INT1_CFG		0x30
#define LIS2DH_REG_INT2_CFG		0x34
#define LIS2DH_AOI_CFG			BIT(7)
#define LIS2DH_INT_CFG_ZHIE_ZUPE	BIT(5)
#define LIS2DH_INT_CFG_ZLIE_ZDOWNE	BIT(4)
#define LIS2DH_INT_CFG_YHIE_YUPE	BIT(3)
#define LIS2DH_INT_CFG_YLIE_YDOWNE	BIT(2)
#define LIS2DH_INT_CFG_XHIE_XUPE	BIT(1)
#define LIS2DH_INT_CFG_XLIE_XDOWNE	BIT(0)

#define LIS2DH_REG_INT2_SRC		0x35

#define LIS2DH_REG_INT2_THS		0x36

#define LIS2DH_REG_INT2_DUR		0x37

/* sample buffer size includes status register */
#define LIS2DH_BUF_SZ			7

#if defined(DT_INST_0_ST_LIS2DH_IRQ_GPIOS_CONTROLLER_1)
/* INT1 and INT2 are configured */
#define DT_LIS2DH_INT1_GPIOS_PIN		DT_INST_0_ST_LIS2DH_IRQ_GPIOS_PIN_0
#define DT_LIS2DH_INT1_GPIOS_FLAGS		DT_INST_0_ST_LIS2DH_IRQ_GPIOS_FLAGS_0
#define DT_LIS2DH_INT1_GPIO_DEV_NAME	DT_INST_0_ST_LIS2DH_IRQ_GPIOS_CONTROLLER_0
#define DT_LIS2DH_INT2_GPIOS_PIN		DT_INST_0_ST_LIS2DH_IRQ_GPIOS_PIN_1
#define DT_LIS2DH_INT2_GPIOS_FLAGS		DT_INST_0_ST_LIS2DH_IRQ_GPIOS_FLAGS_1
#define DT_LIS2DH_INT2_GPIO_DEV_NAME	DT_INST_0_ST_LIS2DH_IRQ_GPIOS_CONTROLLER_1
#else
/* INT1 only */
#define DT_LIS2DH_INT1_GPIOS_PIN		DT_INST_0_ST_LIS2DH_IRQ_GPIOS_PIN
#define DT_LIS2DH_INT1_GPIOS_FLAGS		DT_INST_0_ST_LIS2DH_IRQ_GPIOS_FLAGS
#define DT_LIS2DH_INT1_GPIO_DEV_NAME	DT_INST_0_ST_LIS2DH_IRQ_GPIOS_CONTROLLER
#endif

union lis2dh_sample {
	u8_t raw[LIS2DH_BUF_SZ];
	struct {
		u8_t status;
		s16_t xyz[3];
	} __packed;
};

struct lis2dh_config {
	char *bus_name;
	int (*bus_init)(struct device *dev);
#ifdef DT_ST_LIS2DH_BUS_I2C
	u16_t i2c_slv_addr;
#elif DT_ST_LIS2DH_BUS_SPI
	struct spi_config spi_conf;
#if defined(DT_INST_0_ST_LIS2DH_CS_GPIOS_CONTROLLER)
	const char *gpio_cs_port;
	u8_t cs_gpio;
#endif /* DT_INST_0_ST_LIS2DH_CS_GPIOS_CONTROLLER */
#endif /* DT_ST_LIS2DH_BUS_SPI */

};

struct lis2dh_transfer_function {
	int (*read_data)(struct device *dev, u8_t reg_addr,
			 u8_t *value, u8_t len);
	int (*write_data)(struct device *dev, u8_t reg_addr,
			  u8_t *value, u8_t len);
	int (*read_reg)(struct device *dev, u8_t reg_addr,
			u8_t *value);
	int (*write_reg)(struct device *dev, u8_t reg_addr,
			u8_t value);
	int (*update_reg)(struct device *dev, u8_t reg_addr,
			  u8_t mask, u8_t value);
};

struct lis2dh_data {
	struct device *bus;
	const struct lis2dh_transfer_function *hw_tf;

	union lis2dh_sample sample;
	/* current scaling factor, in micro m/s^2 / lsb */
	u32_t scale;

#ifdef CONFIG_LIS2DH_TRIGGER
	struct device *gpio_int1;
	struct device *gpio_int2;
	struct gpio_callback gpio_int1_cb;
	struct gpio_callback gpio_int2_cb;

	sensor_trigger_handler_t handler_drdy;
	sensor_trigger_handler_t handler_anymotion;
	atomic_t trig_flags;
	enum sensor_channel chan_drdy;

#if defined(CONFIG_LIS2DH_TRIGGER_OWN_THREAD)
	K_THREAD_STACK_MEMBER(thread_stack, CONFIG_LIS2DH_THREAD_STACK_SIZE);
	struct k_thread thread;
	struct k_sem gpio_sem;
#elif defined(CONFIG_LIS2DH_TRIGGER_GLOBAL_THREAD)
	struct k_work work;
	struct device *dev;
#endif

#endif /* CONFIG_LIS2DH_TRIGGER */
#if defined(DT_INST_0_ST_LIS2DH_CS_GPIOS_CONTROLLER)
	struct spi_cs_control cs_ctrl;
#endif /* DT_INST_0_ST_LIS2MDL_CS_GPIOS_CONTROLLER */
};

#if defined(DT_ST_LIS2DH_BUS_SPI)
int lis2dh_spi_access(struct lis2dh_data *ctx, u8_t cmd,
		      void *data, size_t length);
#endif

#ifdef CONFIG_LIS2DH_TRIGGER
int lis2dh_trigger_set(struct device *dev,
		       const struct sensor_trigger *trig,
		       sensor_trigger_handler_t handler);

int lis2dh_init_interrupt(struct device *dev);

int lis2dh_acc_slope_config(struct device *dev, enum sensor_attribute attr,
			    const struct sensor_value *val);
#endif

int lis2dh_spi_init(struct device *dev);
int lis2dh_i2c_init(struct device *dev);


#endif /* __SENSOR_LIS2DH__ */
