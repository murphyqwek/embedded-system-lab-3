#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../Core/Src/oled.c"

I2C_HandleTypeDef hi2c1;
static unsigned transfers, locks, unlocks;
static uint8_t panel[1024];

void I2C1_Lock(void) { locks++; }
void I2C1_Unlock(void) { unlocks++; }
void Error_Handler(void) { assert(!"OLED transfer failed"); }
void HAL_Delay(uint32_t ms) { (void)ms; }
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *bus, uint16_t addr,
    uint16_t reg, uint16_t regSize, uint8_t *data, uint16_t size, uint32_t timeout) {
    (void)bus; (void)addr; (void)regSize; (void)timeout;
    if (reg == 0x40) {
        assert(size == 128);
        memcpy(panel + (transfers % 8) * 128, data, size);
        transfers++;
    }
    return HAL_OK;
}

int main(void) {
    assert(oled_Init());
    assert(transfers == 8 && locks == 1 && unlocks == 1);
    oled_Fill(White);
    oled_UpdateScreen();
    for (int i = 0; i < 1024; i++) assert(panel[i] == 0xFF);
    unsigned before = transfers;
    oled_Fill(Black);
    oled_DrawPixel(127, 63, White);
    assert(transfers == before); /* Clear and draw never expose an empty frame. */
    for (int i = 0; i < 1024; i++) assert(panel[i] == 0xFF);
    oled_UpdateScreen();
    for (int i = 0; i < 1023; i++) assert(panel[i] == 0);
    assert(panel[1023] == 0x80);
    assert(transfers == before + 8 && locks == unlocks);

    oled_SetCursor(121, 54); /* A character exactly touching both edges fits. */
    assert(oled_WriteChar('A', Font_7x10, White) == 'A');
    assert(oled_WriteChar('B', Font_7x10, White) == 0);
    oled_SetCursor(0, 0);
    assert(oled_WriteChar((char)0xFF, Font_7x10, White) == '?');
    puts("PASS: OLED buffer isolation, page transfer, mutex, text bounds");
    return 0;
}
