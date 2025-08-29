#include <stddef.h>
#include <string.h>
#include "GPIO_driver.h"
#include "RTC_driver.h"
#include "I2C_driver.h"

// Private function prototypes
static void ds1307_i2c_pin_config(void);
static void ds1307_i2c_config(void);
static uint8_t ds1307_read(uint8_t reg_addr);
static void ds1307_write(uint8_t value,uint8_t reg_addr);
static uint8_t binary_to_bcd(uint8_t value);
static uint8_t bcd_to_binary(uint8_t value);

// Global I2C handle for DS1307
I2C_Handle_t g_ds1307I2cHandle;

// Initialize DS1307 RTC
uint8_t ds1307_init(void)
{
	ds1307_i2c_pin_config();   // Configure I2C GPIO pins
	ds1307_i2c_config();       // Configure I2C peripheral
	I2C_PeripheralControl(DS1307_I2C, ENABLE); // Enable I2C

	ds1307_write(0x00, DS1307_ADDR_SEC);       // Reset seconds register
	uint8_t clock_state = ds1307_read(DS1307_ADDR_SEC); // Read seconds register
	return ((clock_state >> 7 ) & 0x1);       // Return CH (clock halt) bit
}

// Set current time in DS1307
void ds1307_set_current_time(RTC_time_t *rtc_time)
{
	uint8_t seconds, hrs;
	seconds = binary_to_bcd(rtc_time->seconds);
	seconds &= ~(1 << 7); // Clear CH (clock halt) bit
	ds1307_write(seconds, DS1307_ADDR_SEC);
	ds1307_write(binary_to_bcd(rtc_time->minutes), DS1307_ADDR_MIN);

	hrs = binary_to_bcd(rtc_time->hours);
	if(rtc_time->time_format == TIME_FORMAT_24HRS){
		hrs &= ~(1 << 6); // 24-hour mode
	}else{
		hrs |= (1 << 6);  // 12-hour mode
		hrs = (rtc_time->time_format == TIME_FORMAT_12HRS_PM) ? hrs | (1 << 5) : hrs & ~(1 << 5); // Set AM/PM
	}
	ds1307_write(hrs, DS1307_ADDR_HRS);
}

// Set current date in DS1307
void ds1307_set_current_date(RTC_date_t *rtc_date)
{
	ds1307_write(binary_to_bcd(rtc_date->date), DS1307_ADDR_DATE);
	ds1307_write(binary_to_bcd(rtc_date->month), DS1307_ADDR_MONTH);
	ds1307_write(binary_to_bcd(rtc_date->year), DS1307_ADDR_YEAR);
	ds1307_write(binary_to_bcd(rtc_date->day), DS1307_ADDR_DAY);
}

// Read current time from DS1307
void ds1307_get_current_time(RTC_time_t *rtc_time)
{
	uint8_t seconds, hrs;
	seconds = ds1307_read(DS1307_ADDR_SEC);
	seconds &= ~(1 << 7); // Clear CH bit
	rtc_time->seconds = bcd_to_binary(seconds);
	rtc_time->minutes = bcd_to_binary(ds1307_read(DS1307_ADDR_MIN));

	hrs = ds1307_read(DS1307_ADDR_HRS);
	if(hrs & (1 << 6)){  // 12-hour mode
		rtc_time->time_format = !((hrs & (1 << 5)) == 0); // AM/PM
		hrs &= ~(0x3 << 5);
	}else{
		rtc_time->time_format = TIME_FORMAT_24HRS; // 24-hour mode
	}
	rtc_time->hours = bcd_to_binary(hrs);
}

// Read current date from DS1307
void ds1307_get_current_date(RTC_date_t *rtc_date)
{
	rtc_date->day =  bcd_to_binary(ds1307_read(DS1307_ADDR_DAY));
	rtc_date->date = bcd_to_binary(ds1307_read(DS1307_ADDR_DATE));
	rtc_date->month = bcd_to_binary(ds1307_read(DS1307_ADDR_MONTH));
	rtc_date->year = bcd_to_binary(ds1307_read(DS1307_ADDR_YEAR));
}

// Configure I2C GPIO pins for DS1307
static void ds1307_i2c_pin_config(void)
{
	GPIO_Handle_t i2c_sda, i2c_scl;
	memset(&i2c_sda,0,sizeof(i2c_sda));
	memset(&i2c_scl,0,sizeof(i2c_scl));

	// SDA pin configuration
	i2c_sda.pGPIOx = DS1307_I2C_GPIO_PORT;
	i2c_sda.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	i2c_sda.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_sda.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SDA_PIN;
	i2c_sda.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	i2c_sda.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
	i2c_sda.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOB_PCLK_EN();
	GPIO_Init(&i2c_sda);

	// SCL pin configuration
	i2c_scl.pGPIOx = DS1307_I2C_GPIO_PORT;
	i2c_scl.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	i2c_scl.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_scl.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SCL_PIN;
	i2c_scl.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	i2c_scl.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
	i2c_scl.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&i2c_scl);
}

// Configure I2C peripheral
static void ds1307_i2c_config(void)
{
	g_ds1307I2cHandle.pI2Cx = DS1307_I2C;
	g_ds1307I2cHandle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C_Init(&g_ds1307I2cHandle);
}

// Write a value to DS1307 register
static void ds1307_write(uint8_t value,uint8_t reg_addr)
{
	uint8_t tx[2];
	tx[0] = reg_addr;  // Register address
	tx[1] = value;     // Data to write
	I2C_MasterSendData(&g_ds1307I2cHandle, tx, 2, DS1307_I2C_ADDRESS, 0);
}

// Read a value from DS1307 register
static uint8_t ds1307_read(uint8_t reg_addr)
{
	uint8_t data;
	I2C_MasterSendData(&g_ds1307I2cHandle, &reg_addr, 1, DS1307_I2C_ADDRESS, 0);
	I2C_MasterReceiveData(&g_ds1307I2cHandle, &data, 1, DS1307_I2C_ADDRESS, 0);
	return data;
}

// Convert binary to BCD format
static uint8_t binary_to_bcd(uint8_t value)
{
	uint8_t m , n;
	uint8_t bcd;
	bcd = value;
	if(value >= 10)
	{
		m = value /10;
		n = value % 10;
		bcd = (m << 4) | n;
	}
	return bcd;
}

// Convert BCD to binary format
static uint8_t bcd_to_binary(uint8_t value)
{
	uint8_t m , n;
	m = (uint8_t)((value >> 4) * 10);
	n = value & (uint8_t)0x0F;
	return (m+n);
}
