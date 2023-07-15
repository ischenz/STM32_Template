#include "u8g2_init.h"
#include "delay.h"

u8g2_t u8g2;

/*
	u8g2_init();
	u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_gpio_and_delay_template);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
	u8g2_SetFont(&u8g2,u8g2_font_12x6LED_tf);
*/

void u8g2_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE,ENABLE);//使能PORTE,PORTD时钟

	//GPIO初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOD,GPIO_Pin_10);

	//GPIO初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
}

uint8_t u8x8_gpio_and_delay_template(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch(msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8
            break;							// can be used to setup pins
        case U8X8_MSG_DELAY_NANO:			// delay arg_int * 1 nano second
            break;
        case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
            __NOP();
            break;
        case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
            delay_us(10);
            break;
        case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
            delay_ms(1);
            break;
        case U8X8_MSG_DELAY_I2C:				// arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
            break;							// arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
        case U8X8_MSG_GPIO_D0:				// D0 or SPI clock pin: Output level in arg_int
            //case U8X8_MSG_GPIO_SPI_CLOCK:
            GPIO_WriteBit(GPIOD,GPIO_Pin_8,(BitAction)arg_int);
            break;
        case U8X8_MSG_GPIO_D1:				// D1 or SPI data pin: Output level in arg_int
            //case U8X8_MSG_GPIO_SPI_DATA:
            GPIO_WriteBit(GPIOE,GPIO_Pin_14,(BitAction)arg_int);
            break;
        case U8X8_MSG_GPIO_D2:				// D2 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D3:				// D3 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D4:				// D4 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D5:				// D5 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D6:				// D6 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D7:				// D7 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_E:				// E/WR pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
            GPIO_WriteBit(GPIOE,GPIO_Pin_8,(BitAction)arg_int);
            break;
        case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
            GPIO_WriteBit(GPIOE,GPIO_Pin_10,(BitAction)arg_int);
            break;
        case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
            GPIO_WriteBit(GPIOE,GPIO_Pin_12,(BitAction)arg_int);
            break;
        case U8X8_MSG_GPIO_CS1:				// CS1 (chip select) pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_CS2:				// CS2 (chip select) pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_I2C_CLOCK:		// arg_int=0: Output low at I2C clock pin
            break;							// arg_int=1: Input dir with pullup high for I2C clock pin
        case U8X8_MSG_GPIO_I2C_DATA:			// arg_int=0: Output low at I2C data pin
            break;							// arg_int=1: Input dir with pullup high for I2C data pin
        case U8X8_MSG_GPIO_MENU_SELECT:
            u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_NEXT:
            u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_PREV:
            u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_HOME:
            u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
            break;
        default:
            u8x8_SetGPIOResult(u8x8, 1);			// default return value
            break;
    }
    return 1;
}

