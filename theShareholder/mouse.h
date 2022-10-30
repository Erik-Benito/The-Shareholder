#pragma once
#ifdef __GLOBAL__GAME__
#define __GLOBAL__GAME__

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
	ALLEGRO_BITMAP* image;
} Mouse;


#endif
