#include <stdio.h>

#include <allegro5\allegro.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>


#include "game.h"

Container container;
Background background;

// Prototypes
void LogError(char* error);

void initContainer(Container* container, Background* background);
void EndContainer(Container* container);

void InitBackground(Background* background, float x, float y, int width, int height, ALLEGRO_BITMAP* image);
void DrawBackground(Background* background);

void InitEvent(Container* container, Background* background);
void ControlEvent(Container* container, Background* background);

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


	al_init_ttf_addon();
	al_init_font_addon();
	al_init_image_addon();
	al_init_primitives_addon();

	// Carrega Background
	InitBackground(background, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_load_bitmap("teste1.bmp"));
	DrawBackground(background);

	// Carregando Eventos do game 
	InitEvent(container, background);

};
void EndContainer(Container* container) {
	al_destroy_event_queue(container->eventQueue);
}


void InitBackground(Background* background, float x, float y, int width, int height, ALLEGRO_BITMAP* image)
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

	if (background->x + background->width < WINDOW_WIDTH)
		al_draw_bitmap(background->image, background->x + background->width, background->y, 0);
}

void InitEvent(Container* container, Background* background)
{
	al_install_keyboard();

	container->hasFinished = false;
	container->eventQueue = NULL;

	container->eventQueue = al_create_event_queue();
	if (!container->eventQueue)
		LogError("Nã0 foi possivel carregar o evento");

	al_register_event_source(container->eventQueue, al_get_keyboard_event_source());
	ControlEvent(container, background);
}


int pos_x = 100;
int pos_y = 100;



void ControlEvent(Container* container, Background* background)
{
	int fps = 0;
	ALLEGRO_FONT* font = al_load_font("BAVEUSE.TTF", 20, NULL);

	while (!container->hasFinished)
	{
		al_flip_display();

		DrawBackground(background);
		al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 5, ALLEGRO_ALIGN_CENTRE, "FPS: %d", fps++);

		ALLEGRO_EVENT event;

		al_wait_for_event(container->eventQueue, &event);

		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				container->hasFinished = true;

			switch (event.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				pos_y -= 10;
				break;
			case ALLEGRO_KEY_DOWN:
				pos_y += 10;
				break;
			case ALLEGRO_KEY_RIGHT:
				pos_x += 10;
				break;
			case ALLEGRO_KEY_LEFT:
				pos_x -= 10;
				break;
			}
		}

		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			container->hasFinished = true;

		al_draw_filled_rectangle( pos_x, pos_y, pos_x + 30, pos_y + 30, al_map_rgb(255, 255, 0));


	}

	EndContainer(container);
}