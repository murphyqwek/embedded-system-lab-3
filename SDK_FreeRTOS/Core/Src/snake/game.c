#include "FreeRTOS.h"
#include "snake/field.h"
#include "snake/apple_generator.h"
#include "snake/snake.h"
#include "snake/event.h"
#include "snake/key_queue.h"
#include "snake/game_types.h"
#include "snake/game.h"
#include "portmacro.h"

Snake *snake;
GameState state;

void StartGameTask(void* arguments) {
	EventGroupHandle_t event = getEvent();


	for(;;) {
		//Ждем событий
		EventBits_t bits =  xEventGroupWaitBits(event,
												ALL_EVENTS,
												pdTRUE,
												pdFALSE,
												portMAX_DELAY);

		if(bits & GAME_START_EVENT) {
			StartGame();
		}

		if(bits & GAME_TICK_EVENT) {
			if(GameTick()) {
				clearEvents();

				//TODO: перейти в режим конца игры

				continue;
			}
		}

		if(bits & INPUT_EVENT) {
			//TODO: вызвать функцию ChangeDirection или EnterAnyKey
		}
	}
}

bool GameTick(void) {
	//TODO: прописать логику тика
}

void StartGame(void) {
	FieldInit();
	Snake_Init(snake);

	//TODO: запустить таймер
}




