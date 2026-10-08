#include "snake/field.h"
#include "snake/apple_generator.h"
#include "snake/snake.h"
#include "snake/event.h"
#include "snake/key_queue.h"
#include "snake/game_types.h"
#include "snake/game.h"
#include "snake/render.h"
#include "portmacro.h"

//Я забыл, что мы должны инициализировать
//Мне было впдалу везде переписывать на &snake, поэтому сделаем так
Snake snakeStorage;
Snake *snake = &snakeStorage;
static GameState state = GAME_START_MENU;

Position apple;

TimerHandle_t ticksTimer;
TimerHandle_t startGameTimer;


void timerTicksHandler(TimerHandle_t handle) {
	gameTickEvent();
}

void timerStartGameHandler(TimerHandle_t handle) {
	gameStartEvent();
}

void StartGameTask(void* arguments) {
	state = GAME_START_MENU;

	EventGroupHandle_t event = getEvent();


	//Настраиваем таймеры
	ticksTimer = xTimerCreate("TicksTimer",
					 pdMS_TO_TICKS(TICKS_MS),
					 pdTRUE,
					 (void *) 0,
					 timerTicksHandler);

	startGameTimer = xTimerCreate("StartGameTimer",
								  pdMS_TO_TICKS(START_GAME_DELAY_MS),
								  pdFALSE,
								  (void *) 0,
								  timerStartGameHandler);

	if(startGameTimer == NULL || ticksTimer == NULL) {
		error();
	}

	renderStartMenu();

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
				xTimerStop(ticksTimer, 0);
				clearEvents();

				EndGameState endState = state == GAME_WIN ? WIN : LOSE;

				renderEndGameMenu(endState);

				continue;
			}
		}

		if(bits & INPUT_EVENT) {
			int key = readKeyFromQueue();

			if(state == GAME_RUNNING ||
			   state == GAME_STARTING) {
				changeDiretion(key);

				continue;
			}

			enterAnyKey();
		}
	}
}

void enterAnyKey(void) {
	//По сути, мы из любого состояния мы должны перейти в
	//начало игры

	FieldInit();
	Snake_Init(snake);

	if(!generateNewApplePosition(&apple)) {
		error();
	}

	FieldSet(apple, CELL_APPLE);

	renderStartField();

	state = GAME_STARTING;

	xTimerStart(startGameTimer, 0);
}

void changeDiretion(int key) {
	Snake_SetNewDirection(snake, key);
}

//Возвращает true, если нам нужно очистить событие
bool GameTick(void) {
	//TODO: прописать логику тика

	//Проверка, что голова находится на яблоке
	if(comparePositions(snake->head->pos, apple)) {
		//Пытаемся увеличить змейку
		if(!Snake_Grow(snake)) {
			//Не удалось - коллизия
			state = GAME_OVER;

			return true;
		}

		//Удалось увеличить
		//Проверяем на победу
		if(snake->length == FIELD_CELLS) {
			state = GAME_WIN;
			return true;
		}

		//Генерируем новое яблоко
		if(generateNewApplePosition(&apple)) {
			FieldSet(apple, CELL_APPLE);
		}
		else {
			//По каким-то причинам не удалось создать яблоко
			error();
		}

		return false;
	}

	//Пытаемся переместить змейку
	if(!Snake_Move(snake)) {
		//Коллизия
		state = GAME_OVER;

		return true;
	}

	return false;
}

void StartGame(void) {
	state = GAME_RUNNING;

	//Запускаем таймер
	xTimerStart(ticksTimer, 0);
}


void error() {
	if(startGameTimer != NULL) {
		xTimerStop(startGameTimer, 0);
	}

	if(ticksTimer != NULL) {
		xTimerStop(ticksTimer, 0);
	}

	clearEvents();
	renderError();

	vTaskSuspend(NULL);
}



