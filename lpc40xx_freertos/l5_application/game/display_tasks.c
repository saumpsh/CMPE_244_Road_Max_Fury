
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "display_tasks.h"
#include "graphics.h"
#include "led_matrix.h"
#include "object.h"
#include "queue.h"
#include "semphr.h"

extern uint8_t game_screen_state;
extern SemaphoreHandle_t countdown;
extern SemaphoreHandle_t crash;
extern SemaphoreHandle_t level;
extern SemaphoreHandle_t car_moving;
extern SemaphoreHandle_t play;
extern SemaphoreHandle_t no_sound;

static void draw_game_screen() {
  generate_random_obstacles();
  move();
  draw();
  collision_detector();
}

void display_task(void *params) {

  object__init_player_car();
  game_screen_state = START_SCREEN;

  while (true) {
    switch (game_screen_state) {
    case START_SCREEN:
      xSemaphoreGive(no_sound);
      draw_start_screen();
      break;
    case GAME_SCREEN:
      xSemaphoreGive(car_moving);
      draw_game_screen();
      break;
    case CAR_CRASH:
      xSemaphoreGive(crash);
      draw_crash_screen();
      break;
    }

    vTaskDelay(50);
  }
}

// smiley
static const uint8_t smiley[] = {0x3c, 0x42, 0x99, 0xa5, 0x81,
                                 0xa5, 0x81, 0x42, 0x3c};

void test_graphics_task(void *params) {

  // drawLine(0, 0, 31, 63, red_4);
  // drawRect(0, 0, 10, 20, white_7);
  // fillRect(0, 0, 20, 10, YELLOW);
  // drawChar(5, 5, 'R', red_4, red_4, 1);
  drawBitmap(15, 31, smiley, 8, 9, RED);

  while (true) {
    // draw_car(player_car);
    vTaskDelay(1);
  }
}
