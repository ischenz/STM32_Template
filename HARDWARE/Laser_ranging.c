#include "Laser_ranging.h"
#include "sw_i2c.h"
#include "delay.h"
#include "usart.h"

/* �������IIC���� */
sw_i2c_interface_t i2c_interface = 
{
	.sda_in = sda_in,
	.scl_out = scl_out,
	.sda_out = sda_out,
	.user_data = 0, //�û����ݣ������������������õ�
};
void Laser_ranging_init(void)
{
	/* ���ɨ�赽�ĵ�ַ */
	uint8_t scan_addr[128] = {0};
	volatile uint8_t count;
	uint8_t ping_response;
	/* ��ʼ��IIC */
	sw_i2c_init();
	/* ps: ���IIC��ʼ����ʱ������һ��IIC start���ᵼ�µ�һ��IICͨѶ��ʧ�� */
	
	/* ��һ��IICͨѶ��ʧ�ܣ���Ϊ���IIC������start�����ֶ�����stopҲ������ */
	sw_i2c_mem_read(&i2c_interface, 0x4C << 1, 0x55, &ping_response, 1);
	/* ����IICͨѶ�������� */
	sw_i2c_mem_read(&i2c_interface, 0x4C << 1, 0x55, &ping_response, 1);

	count = i2c_scan(&i2c_interface, scan_addr);
	printf("count=%d, addr=%#X\r\n", count, scan_addr[0]);
	
	//sw_i2c_write_byte(&i2c_interface, 0x4C << 1, GW_GRAY_DIGITAL_MODE);
	/* ��ȡ���������� */
	//sw_i2c_read_byte(&i2c_interface, 0x4C << 1, &digital_data); // digital_data ��1~8��̽ͷ��������
	//* ���ֽ����8���������浽�˸����������Ϊgray_sensor[0] ~ gray_sensor[7], 
}

void Laser_ranging_getdistance(uint16_t *__dis)
{
	uint8_t ldata, hdata;
	uint16_t data;
	sw_i2c_mem_read(&i2c_interface, 0x32 << 1, 0x01, &hdata, 1);
	sw_i2c_mem_read(&i2c_interface, 0x32 << 1, 0x02, &ldata, 1);
	data = hdata << 8 | ldata;
	*__dis = data;
}

/**
 * ��ʼ��i2c
 */
void sw_i2c_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = SW_I2C1_PIN_SCL | SW_I2C1_PIN_SDA;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC, SW_I2C1_PIN_SCL | SW_I2C1_PIN_SDA);
}

/* ����sda������� bit=0Ϊ�͵�ƽ bit=1Ϊ�ߵ�ƽ */
void sda_out(uint8_t bit, void *user_data)
{
	SDA_OUT();
	GPIO_WriteBit(GPIOC, SW_I2C1_PIN_SDA, (BitAction)bit);
	
	/* IIC����ӳ� */
	delay_us(10);
}

/* ����sda��ȡ���� bit Ϊ���صĵ�ƽֵ */
uint8_t sda_in(void *user_data)
{
	uint8_t bit;
	SDA_IN();
	bit = (uint8_t)GPIO_ReadInputDataBit(GPIOC, SW_I2C1_PIN_SDA);
	
	/* IIC����ӳ� */
	delay_us(10);
	return bit;
}

/* ����sclʱ��������� bit=0Ϊ�͵�ƽ bit=1Ϊ�ߵ�ƽ */
void scl_out(uint8_t bit, void *user_data)
{
	GPIO_WriteBit(GPIOC, SW_I2C1_PIN_SCL, (BitAction)bit);
	
	/* IIC����ӳ� */
	delay_us(10);
}

/**
 * i2c��ַɨ��
 * @param scan_addr ɨ������ĵ�ַ���,��ֵ��Ϊ0��Ϊɨ�赽�ĵ�ַ��ɨ���ĵ�ַ�ᰤ�������������ǰ��
 * @return ����ɨ�赽���豸����, 0Ϊ���豸����
 */
uint8_t i2c_scan(sw_i2c_interface_t *i2c_interface, uint8_t *scan_addr)
{
	int i;
	uint8_t count = 0;
	uint8_t data;
	int8_t ret;
	
	for (i = 1; i < 127; ++i) {
		ret = sw_i2c_read(i2c_interface, i << 1, &data, 1);
		if (ret == 0) {
			scan_addr[count] = i;
			++count;
		}
	}
	
	return count;
}

