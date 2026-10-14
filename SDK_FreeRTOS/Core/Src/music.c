#include "music.h"
#include "buzzer.h"
#include "FreeRTOS.h"
#include "task.h"

/* Melody and tempo from the neighbouring SDK_Buzzer project, stored in flash. */
static const uint16_t megalovania_melody[] = {
    N_D3, N_D3, N_D4, N_A3, 0, N_GS3, N_G3, N_F3, N_D3, N_F3, N_G3, N_C3,
    N_C3, N_D4, N_A3, 0, N_GS3, N_G3, N_F3, N_D3, N_F3, N_G3, N_B2, N_B2,
    N_D4, N_A3, 0, N_GS3, N_G3, N_F3, N_D3, N_F3, N_G3, N_AS2, N_AS2, N_D4,
    N_A3, 0, N_GS3, N_G3, N_F3, N_D3, N_F3, N_G3, N_D3, N_D3, N_D4, N_A3,
    0, N_GS3, N_G3, N_F3, N_D3, N_F3, N_G3, N_C3, N_C3, N_D4, N_A3, 0,
    N_GS3, N_G3, N_F3, N_D3, N_F3, N_G3, N_B2, N_B2, N_D4, N_A3, 0, N_GS3,
    N_G3, N_F3, N_D3, N_F3, N_G3, N_AS2, N_AS2, N_D4, N_A3, 0, N_GS3, N_G3,
    N_F3, N_D3, N_F3, N_G3, N_D4, N_D4, N_D5, N_A4, 0, N_GS4, N_G4, N_F4,
    N_D4, N_F4, N_G4, N_C4, N_C4, N_D5, N_A4, 0, N_GS4, N_G4, N_F4, N_D4,
    N_F4, N_G4, N_B3, N_B3, N_D5, N_A4, 0, N_GS4, N_G4, N_F4, N_D4, N_F4,
    N_G4, N_AS3, N_AS3, N_D5, N_A4, 0, N_GS4, N_G4, N_F4, N_D4, N_F4, N_G4,
    N_D4, N_D4, N_D5, N_A4, 0, N_GS4, N_G4, N_F4, N_D4, N_F4, N_G4, N_C4,
    N_C4, N_D5, N_A4, 0, N_GS4, N_G4, N_F4, N_D4, N_F4, N_G4, N_B3, N_B3,
    N_D5, N_A4, 0, N_GS4, N_G4, N_F4, N_D4, N_F4, N_G4, N_AS3, N_AS3, N_D5,
    N_A4, 0, N_GS4, N_G4, N_F4, N_D4, N_F4, N_G4, N_F4, N_F4, N_F4, N_F4,
    N_F4, N_D4, N_D4, N_D4, N_F4, N_F4, N_F4, N_G4, N_GS4, N_G4, N_F4, N_D4,
    N_F4, N_G4, 0, N_F4, N_F4, N_F4, N_G4, N_GS4, N_A4, N_C5, N_A4, N_D5,
    N_D5, N_D5, N_A4, N_D5, N_C5, N_F4, N_F4, N_F4, N_F4, N_F4, N_D4, N_D4,
    N_D4, N_F4, N_F4, N_F4, N_F4, N_D4, N_F4, N_E4, N_D4, N_C4, 0, N_G4,
    N_E4, N_D4, N_D4, N_D4, N_D4, N_F3, N_G3, N_AS3, N_C4, N_D4, N_F4, N_C5,
    0, N_F4, N_D4, N_F4, N_G4, N_GS4, N_G4, N_F4, N_D4, N_GS4, N_G4, N_F4,
    N_D4, N_F4, N_F4, N_F4, N_GS4, N_A4, N_C5, N_A4, N_GS4, N_G4, N_F4, N_D4,
    N_E4, N_F4, N_G4, N_A4, N_C5, N_CS5, N_GS4, N_GS4, N_G4, N_F4, N_G4, N_F3,
    N_G3, N_A3, N_F4, N_E4, N_D4, N_E4, N_F4, N_G4, N_E4, N_A4, N_A4, N_G4,
    N_F4, N_DS4, N_CS4, N_DS4, 0, N_F4, N_D4, N_F4, N_G4, N_GS4, N_G4, N_F4,
    N_D4, N_GS4, N_G4, N_F4, N_D4, N_F4, N_F4, N_F4, N_GS4, N_A4, N_C5, N_A4,
    N_GS4, N_G4, N_F4, N_D4, N_E4, N_F4, N_G4, N_A4, N_C5, N_CS5, N_GS4, N_GS4,
    N_G4, N_F4, N_G4, N_F3, N_G3, N_A3, N_F4, N_E4, N_D4, N_E4, N_F4, N_G4,
    N_E4, N_A4, N_A4, N_G4, N_F4, N_DS4, N_CS4, N_DS4,
};

static const uint16_t megalovania_delays[] = {
    16, 16, 8, 6, 32, 8, 8, 8, 16, 16, 16, 16,
    16, 8, 6, 32, 8, 8, 8, 16, 16, 16, 16, 16,
    8, 6, 32, 8, 8, 8, 16, 16, 16, 16, 16, 8,
    6, 32, 8, 8, 8, 16, 16, 16, 16, 16, 8, 6,
    32, 8, 8, 8, 16, 16, 16, 16, 16, 8, 6, 32,
    8, 8, 8, 16, 16, 16, 16, 16, 8, 6, 32, 8,
    8, 8, 16, 16, 16, 16, 16, 8, 6, 32, 8, 8,
    8, 16, 16, 16, 16, 16, 8, 6, 32, 8, 8, 8,
    16, 16, 16, 16, 16, 8, 6, 32, 8, 8, 8, 16,
    16, 16, 16, 16, 8, 6, 32, 8, 8, 8, 16, 16,
    16, 16, 16, 8, 6, 32, 8, 8, 8, 16, 16, 16,
    16, 16, 8, 6, 32, 8, 8, 8, 16, 16, 16, 16,
    16, 8, 6, 32, 8, 8, 8, 16, 16, 16, 16, 16,
    8, 6, 32, 8, 8, 8, 16, 16, 16, 16, 16, 8,
    6, 32, 8, 8, 8, 16, 16, 16, 8, 16, 8, 8,
    8, 8, 4, 16, 8, 16, 8, 8, 8, 16, 16, 16,
    16, 16, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8,
    8, 16, 16, 16, 2, 8, 16, 8, 8, 8, 8, 4,
    16, 8, 16, 8, 8, 8, 8, 8, 16, 8, 16, 8,
    8, 8, 8, 8, 8, 8, 16, 8, 15, 8, 8, 2,
    3, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 8, 2, 16, 8, 16, 8, 16, 16, 16, 16, 16,
    16, 8, 8, 8, 8, 8, 8, 16, 16, 16, 2, 8,
    8, 8, 8, 4, 4, 4, 4, 4, 4, 2, 8, 8,
    8, 8, 2, 2, 3, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 8, 2, 16, 8, 16, 8, 16,
    16, 16, 16, 16, 16, 8, 8, 8, 8, 8, 8, 16,
    16, 16, 2, 8, 8, 8, 8, 4, 4, 4, 4, 4,
    4, 2, 8, 8, 8, 8, 2, 1,
};

_Static_assert(sizeof(megalovania_melody) == sizeof(megalovania_delays),
               "Every note must have a duration");

void StartMusicTask(void *argument) {
    (void)argument;
    Buzzer_Init();


    for (;;) {
        for (unsigned int i = 0;
             i < sizeof(megalovania_melody) / sizeof(megalovania_melody[0]); ++i) {
            Buzzer_SetTone(megalovania_melody[i] * 4U);
            vTaskDelay(pdMS_TO_TICKS(1920U / megalovania_delays[i]));
            Buzzer_SetTone(0);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
