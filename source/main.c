#include <3ds.h>
#include <citro2d.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "List.h"

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define MOVEMENT_SPEED 10

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

int main(int argc, char **argv)
{
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);
	srand(time(NULL));
	 
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	u32 clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
	u32 clrClear = C2D_Color32(0x00, 0x00, 0x00, 0x00);
	u32 clrRed = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);

	u64 current_time = osGetTime();
	bool game_running = true;

	enum Direction current_direction = RIGHT;
	enum Direction changeto = LEFT;
	Coordinate snake_pos = {50, 50};
	ListPtr snake_body = createList();
	Coordinate food_pos = {100, 40};
	bool food_spawn = true;
	u32 score = 0;

	// prep snake body linked list
	appendList(snake_body, (Coordinate){50, 50});
	appendList(snake_body, (Coordinate){40, 50});
	appendList(snake_body, (Coordinate){30, 50});

	// Main loop
	while (aptMainLoop())
	{
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) break; // break in order to return to hbmenu
		if (kDown & KEY_DLEFT) changeto = LEFT;
		if (kDown & KEY_DRIGHT) changeto = RIGHT;
		if (kDown & KEY_DUP) changeto = UP;
		if (kDown & KEY_DDOWN) changeto = DOWN;

		if (game_running && osGetTime() >= current_time + 125) {
	
			if (changeto == RIGHT && current_direction != LEFT) current_direction = changeto;
			if (changeto == LEFT && current_direction != RIGHT) current_direction = changeto;
			if (changeto == UP && current_direction != DOWN) current_direction = changeto;
			if (changeto == DOWN && current_direction != UP) current_direction = changeto;

			if (current_direction == RIGHT) snake_pos.x_coord += MOVEMENT_SPEED;
			if (current_direction == LEFT) snake_pos.x_coord -= MOVEMENT_SPEED;
			if (current_direction == UP) snake_pos.y_coord -= MOVEMENT_SPEED;
			if (current_direction == DOWN) snake_pos.y_coord += MOVEMENT_SPEED;

			insertAtFrontOfList(snake_body, snake_pos);
			if ((snake_pos.x_coord <= food_pos.x_coord + 5) && (snake_pos.x_coord >= food_pos.x_coord - 5) &&
				(snake_pos.y_coord <= food_pos.y_coord + 5) && (snake_pos.y_coord >= food_pos.y_coord - 5)) {
				food_spawn = false;
				score += 1;
			} else {
				deleteList(snake_body, lengthList(snake_body)-1);
			}

			if (food_spawn == false) {
				u32 food_x = 50 + (rand() % 200);
				u32 food_y = 50 + (rand() % 150);
				food_pos.x_coord = food_x;
				food_pos.y_coord = food_y;
				food_spawn = true;
			}

			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, clrClear);
			C2D_SceneBegin(top);

			for (int i = 0; i < lengthList(snake_body); i++) {
				Coordinate current_body = getList(snake_body, i);
				C2D_DrawRectSolid(current_body.x_coord, current_body.y_coord, 0, 10, 10, clrWhite);
			}
			C2D_DrawRectSolid(food_pos.x_coord, food_pos.y_coord, 0, 10, 10, clrRed);

			printf("\x1b[16;10HScore: %lu", score);
			
			if (snake_pos.x_coord >= SCREEN_WIDTH || snake_pos.x_coord < 0) {
				consoleClear();
				game_running = false;
				printf("\x1b[16;10HGame Over!");
				printf("\x1b[18;10HSTART to quit");
				printf("\x1b[20;10HSELECT to restart");
			}
			if (snake_pos.y_coord >= SCREEN_HEIGHT || snake_pos.y_coord < 0) {
				consoleClear();
				game_running = false;
				printf("\x1b[16;10HGame Over!");
				printf("\x1b[18;10HSTART to quit");
				printf("\x1b[20;10HSELECT to restart");
			}
			for (int i = 1; i < lengthList(snake_body); i++) {
				Coordinate current_body = getList(snake_body, i);
				if (current_body.x_coord == snake_pos.x_coord && current_body.y_coord == snake_pos.y_coord) {
					consoleClear();
					game_running = false;
					printf("\x1b[16;10HGame Over!");
					printf("\x1b[18;10HSTART to quit");
					printf("\x1b[20;10HSELECT to restart");
				}
			}

			C3D_FrameEnd(0);
			current_time = osGetTime();
		} else {
			if (kDown & KEY_SELECT) {
				consoleClear();
				current_direction = RIGHT;
				changeto = LEFT;
				snake_pos = (Coordinate){50, 50};
				destroyList(&snake_body);
				snake_body = createList();
				appendList(snake_body, (Coordinate){50, 50});
				appendList(snake_body, (Coordinate){40, 50});
				appendList(snake_body, (Coordinate){30, 50});
				food_pos = (Coordinate){100, 40};
				food_spawn = true;
				score = 0;
				game_running = true;
			}
		}
	}
	destroyList(&snake_body);
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}
