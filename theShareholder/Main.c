#include <stdio.h>

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

#include "game.h"

Container container;
Background background;

// Prototypes
void LogError(char* error);

void initContainer(Container container);

void InitBackground(Background background, float x, float y, int width, int height, ALLEGRO_BITMAP* image);
void DrawBackground(Background background);

int main(void) {

	//Allegro varivaies
	ALLEGRO_DISPLAY* window = NULL;
	ALLEGRO_BITMAP* bgImage = NULL;

	initContainer(container);

	InitBackground(background, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_load_bitmap("FUNDOteste1.png"));
	DrawBackground(background);


	return 0;
};



void LogError(char* error)
{
	al_show_native_message_box(NULL, "Aviso!", "ERROR:", error, NULL, ALLEGRO_MESSAGEBOX_ERROR);
	return;
};


void initContainer(Container container) 
{
	//Inicia o alegro
	if (!al_init())
		LogError("Falha ao carregar o allegro");

	//Criacao do Display
	if (!(container.window = al_create_display(WINDOW_HEIGHT, WINDOW_WIDTH)))
		LogError("Falha ao carregar janela");

	al_init_primitives_addon();
	al_init_image_addon();

	// Criamos um back buffer
	al_flip_display();
};


void InitBackground(Background background, float x, float y, int width, int height, ALLEGRO_BITMAP* image)
{
	background.x = x;
	background.y = y;
	background.width = width;
	background.height = height;
	background.image = image;
}
void DrawBackground(Background background)
{

	if (background.x + background.width < WINDOW_HEIGHT)
		al_draw_bitmap(background.image, background.x + background.width, background.y, 0);
}