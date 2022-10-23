#pragma once
#ifndef __GLOBAL__GAME__
#define __GLOBAL__GAME__

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 650
#define FPS 30


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

	int speed;
	
	int sourceX;
	int sourceY;
	bool needRedraw;

	ALLEGRO_BITMAP *spritePlayer;

} Player;

typedef struct {

	float hours;
	float minutes;
	float seconds;

	int hoursTotal;
	int minutesTotal;
	int secondsTotal;

	int days;

} TimerGame;



typedef struct {

	bool hasFinished;
	bool needRedraw;

	ALLEGRO_DISPLAY* window;
	ALLEGRO_EVENT_QUEUE* eventQueue;

} Container;



typedef struct {
	int x;
	int y;
	int speed;

	bool active;

	ALLEGRO_BITMAP* image;
} Cloud;



typedef struct {

	float x;
	float y;

	int width;
	int height;

	Cloud cloud[5];

	ALLEGRO_BITMAP* image;

} Background;

#endif
