#include "kb.h"
#include "pca9538.h"

#define KBRD_ADDR 0xE2

HAL_StatusTypeDef Set_Keyboard(void) {
    uint8_t zero = 0;
    HAL_StatusTypeDef result =
        PCA9538_Write_Register(KBRD_ADDR, POLARITY_INVERSION, &zero);
    if (result != HAL_OK) return result;
    return PCA9538_Write_Register(KBRD_ADDR, OUTPUT_PORT, &zero);
}

/* Caller holds the I2C mutex for the complete keyboard scan. */
uint8_t Check_Row(uint8_t row) {
    uint8_t inputs = 0x70;
    if (Set_Keyboard() != HAL_OK ||
        PCA9538_Write_Register(KBRD_ADDR, CONFIG, &row) != HAL_OK ||
        PCA9538_Read_Inputs(KBRD_ADDR, &inputs) != HAL_OK) return 0;
    switch (inputs & 0x70) {
        case 0x60: return 0x04;
        case 0x50: return 0x02;
        case 0x30: return 0x01;
        default: return 0; /* No button or ambiguous multiple buttons. */
    }
}
