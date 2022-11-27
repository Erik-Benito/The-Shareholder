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

	int fpsPlayer;
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
	bool isPcMode;

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
	int x;
	int y;

	int width;
	int height;

	ALLEGRO_BITMAP* image;
} PcBackground;

typedef struct {
	int x;
	int y;

	int width;
	int height;

	ALLEGRO_BITMAP* image;
} Office2Background;

typedef struct {

	float x;
	float y;

	int width;
	int height;

	Cloud cloud[5];
	PcBackground pcBackground;
	Office2Background office2background;

	ALLEGRO_BITMAP* image;

} Background;


typedef struct {

	int amount;

	int lossPerDay;
	int profitPerDay;
	
	int investedProfit;
	
	int safeInvestedAmount;
	int InsecureInvestedAmount;

	int branches;
	int products;
	int employers;

	int valueCompany;

	int chanceToWin;
	int valueToWin;

} Wallet;


#endif
