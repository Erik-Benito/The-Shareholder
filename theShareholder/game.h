#pragma once
#ifndef __GLOBAL__GAME__
#define __GLOBAL__GAME__

#define WINDOW_HEIGHT 832
#define WINDOW_WIDTH 704
#define FPS 60


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
	int mvSpeed;
} Player;


typedef struct {
	bool hasFinished;
	bool needRedraw;
	ALLEGRO_DISPLAY* window;
	ALLEGRO_EVENT_QUEUE* eventQueue;
} Container;


typedef struct {
	float x;
	float y;

	int width;
	int height;

	ALLEGRO_BITMAP* image;
} Background;

#endif
