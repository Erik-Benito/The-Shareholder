#include <stdio.h>

#include <allegro5\allegro.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>


#include "game.h"

Player player;
Container container;
Background background;

// Prototypes
void LogError(char* error);
void LogFrames(int fps);

void InitContainer(Container* container, Background* background, Player* player);
void EndContainer(Container* container);

void InitBackground(Background* background, float x, float y, int width, int height, ALLEGRO_BITMAP* image);
void DrawBackground(Background* background);

void InitPlayer(Player* player);
void PositionPlayer(Player* player, int x, int y);

void InitEvent(Container* container, Background* background, Player* player);
void ControlEvent(Container* container, Background* background, Player* player);

int main(void) {
	InitContainer(&container, &background, &player);
	return 0;
};


void LogError(char* error)
{
	al_show_native_message_box(NULL, "Aviso!", "ERROR:", *error, NULL, ALLEGRO_MESSAGEBOX_ERROR);
	return;
};
void LogFrames(int fps, ALLEGRO_FONT* font)
{
	al_draw_textf(font, al_map_rgb(255, 255, 255), 50, 10, ALLEGRO_ALIGN_CENTRE, "FPS: %d", fps);
}


void InitPlayer(Player* player) {
	player->x = 100;
	player->y = 100;
};
void PositionPlayer(Player* player) {

	al_draw_filled_rectangle(player->x, player->y, player->x + 30, player->y + 30, al_map_rgb(255, 255, 0));
	return;
}


void InitContainer(Container* container, Background* background, Player* player)
{
	//Inicia o alegro
	if (!al_init())
		LogError("Falha ao carregar o allegro");

	//Criacao do Display
	if (!(container->window = al_create_display(WINDOW_HEIGHT, WINDOW_WIDTH)))
		LogError("Falha ao carregar janela");


	// Add-on Allegro
	al_init_ttf_addon();
	al_init_font_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_init_primitives_addon();

	// Carrega Background
	InitBackground(background, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_load_bitmap("teste1.bmp"));
	DrawBackground(background);

	// Criando Personagem
	InitPlayer(player);

	// Carregando Eventos do game 
	InitEvent(container, background, player);

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


void InitEvent(Container* container, Background* background, Player* player)
{
	container->hasFinished = false;
	container->needRedraw = true;

	container->eventQueue = NULL;
	container->eventQueue = al_create_event_queue();
	
	if (!container->eventQueue)
		LogError("Nã0 foi possivel carregar o evento");

	al_register_event_source(container->eventQueue, al_get_keyboard_event_source());
	ControlEvent(container, background, player);
}


void ControlEvent(Container* container, Background* background, Player* player)
{
	int frame = 0;
	double startTime = al_get_time();
	ALLEGRO_FONT* font = al_load_font("BAVEUSE.TTF", 20, NULL);

	while (!container->hasFinished)
	{
		DrawBackground(background);
		PositionPlayer(player);
		LogFrames(frame, font);
		
		al_flip_display();

		ALLEGRO_EVENT event;
		al_wait_for_event(container->eventQueue, &event);

		if (event.type == ALLEGRO_EVENT_KEY_UP)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				container->hasFinished = true;

			switch (event.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				player->y -= 30;
				break;
			case ALLEGRO_KEY_DOWN:
				player->y += 30;
				break;
			case ALLEGRO_KEY_RIGHT:
				player->x += 30;
				break;
			case ALLEGRO_KEY_LEFT:
				player->x -= 30;
				break;
			}
		}

		frame++;
		if ((al_get_time() - startTime) < 1.0 / FPS)
			al_rest((1.0 / FPS) - (al_get_time() - startTime));
		else if (frame > 60)
			frame--;
	}

	EndContainer(container);
}