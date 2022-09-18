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

void initContainer(Container* container);

void InitBackground(Background* background, float x, float y, int width, int height, ALLEGRO_BITMAP *image);
void DrawBackground(Background* background);

int main(void) {
	initContainer(&container, &background);
	return 0;
};



void LogError(char* error)
{
	al_show_native_message_box(NULL, "Aviso!", "ERROR:", *error, NULL, ALLEGRO_MESSAGEBOX_ERROR);
	return;
};


void initContainer(Container* container, Background* background)
{
	//Inicia o alegro
	if (!al_init())
		LogError("Falha ao carregar o allegro");

	//Criacao do Display
	if (!(container->window = al_create_display(WINDOW_HEIGHT, WINDOW_WIDTH)))
		LogError("Falha ao carregar janela");

	// Carrega add-on
	al_init_primitives_addon();
	al_init_image_addon();

	// Carrega Background
	InitBackground(&background, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_load_bitmap("teste1.bmp"));
	DrawBackground(&background);

	// Criamos um back buffer
	al_flip_display();

	al_rest(5.0);
};


void InitBackground(Background* background, float x, float y, int width, int height, ALLEGRO_BITMAP *image)
{
	background->image = NULL;

	background->x = x;
	background->y = y;
	background->width = width;
	background->height = height;
	background->image = image;

	if (!background->image)
		LogError("Falha ao carregar a imagem");
}
void DrawBackground(Background* background)
{
	al_draw_bitmap(background->image, background->x, background->y, 0);

	if (background->x + background->width < WINDOW_HEIGHT)
		al_draw_bitmap(background->image, background->x + background->width, background->y, 0);
}