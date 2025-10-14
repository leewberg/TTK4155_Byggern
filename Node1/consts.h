// #define FOSC 4915200
// #define F_CPU 4915200
#define FOSC 16000000UL
#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR (FOSC / (16UL* BAUD) - 1)
#define set_bit (reg, bit)(reg |= (1 << bit))
#define clear_bit (reg, bit)
#define test_bit (reg, bit)(reg & (1 << bit))
// #define loop_until_bit_is_set(reg, bit) while ( !test_bit( reg, bit ))
// #define loop_until_bit_is_clear(reg, bit) while ( test_bit( reg, bit ))
#define OCR1A_VAL 0b11
#define FREQ_ADC FOSC / (2 * (1 + OCR1A_VAL))
#define CONVERSION_TIME (9 * 2 * 4) / FREQ_ADC * 1000000 // in microseconds
#define ADC_ADDR 0x1300
#define JOY_DEADZONE 3
#define Dx_min 51
#define Dx_mid 128
#define Dx_max 205
#define Dy_min 51
#define Dy_mid 128
#define Dy_max 205
#define OLED_ON 0b10101111
#define OLED_OFF 0b10101110
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
