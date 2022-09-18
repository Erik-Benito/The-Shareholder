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

void initContainer(Container* container, Background *background);
void EndContainer(Container* container);

void InitBackground(Background* background, float x, float y, int width, int height, ALLEGRO_BITMAP *image);
void DrawBackground(Background* background);

void InitEvent(Container* container);
void ControlEvent(Container* container);

double tempoInicial;

void iniciarTimer()
{
	tempoInicial = al_get_time();
}

double obterTempoTimer()
{
	return al_get_time() - tempoInicial;
}

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

	al_flip_display();
	al_init_primitives_addon();
	al_init_image_addon();

	al_init_font_addon();

	// Carrega Background
	InitBackground(&background, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_load_bitmap("teste1.bmp"));
	DrawBackground(&background);

	// Carregando Eventos do game 
	InitEvent(&container);

};
void EndContainer(Container* container) {
	al_flip_display(container->window);
	al_destroy_event_queue(container->eventQueue);
}


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

	if (background->x + background->width < WINDOW_WIDTH)
		al_draw_bitmap(background->image, background->x + background->width, background->y, 0);

	// Criamos um back buffer
	al_flip_display();
}


void InitEvent(Container *container)
{
	al_install_keyboard();

	container->hasStarted = true;
	container->eventQueue = NULL;

	container->eventQueue = al_create_event_queue();
	if (!container->eventQueue)
		LogError("Nã0 foi possivel carregar o evento");

	al_register_event_source(container->eventQueue, al_get_keyboard_event_source());
	ControlEvent(container);
}
void ControlEvent(Container* container)
{

	int frame = 0;
	bool limitado = true;

	while(container->hasStarted)
	{
		iniciarTimer();
		
		ALLEGRO_EVENT event;

		al_wait_for_event(container->eventQueue, &event);

		if(event.type == ALLEGRO_EVENT_KEY_UP)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				container->hasStarted = false;
			if (event.type == ALLEGRO_EVENT_KEY_UP)
			{
				if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
				{
					limitado = !limitado;
				}
			}
		}
	
		// Criamos um back buffer
		al_flip_display();
	}

	frame++;

	al_flip_display();
	if (limitado && (obterTempoTimer() < 1.0 / 60))
	{
		al_rest((1.0 / 60) - obterTempoTimer());
	}

	EndContainer(container);
}