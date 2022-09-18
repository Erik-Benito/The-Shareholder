#pragma once
#ifndef __GLOBAL__GAME__

#define __GLOBAL__GAME__
#define WINDOW_HEIGHT 680
#define WINDOW_WIDTH 640

#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

typedef struct {
	int x;
	int y;
} Player;

typedef struct {
	ALLEGRO_DISPLAY* window;
	ALLEGRO_BITMAP* background;
} Container;

typedef struct {
	float x;
	float y;

	int width;
	int height;

	ALLEGRO_BITMAP* image;
} Background;



#endif
