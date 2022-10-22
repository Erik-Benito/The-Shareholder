#include <stdio.h>
#include <time.h>

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

void fadeInNight(Background* background, float speed);

void MovingCloudBackground(Cloud cloud[]);
void DrawCloudBackground(Cloud cloud[]);
void attCloudPosition(Cloud cloud[]);

void InitPlayer(Player* player);
void PositionPlayer(Player* player, int x, int y);

void InitEvent(Container* container, Background* background, Player* player);
void ControlEvent(Container* container, Background* background, Player* player);


int main(void) {
	InitContainer(&container, &background,&player);
	return 0;
};

void LogError(char* error)
{
	al_show_native_message_box(NULL, "Aviso!", "ERROR:", *error, NULL, ALLEGRO_MESSAGEBOX_ERROR);
	return;
};
void LogFrames(int fps, ALLEGRO_FONT* font)
{
	al_draw_textf(font, al_map_rgb(0, 0, 0), 50, 10, ALLEGRO_ALIGN_CENTRE, "FPS: %d", fps);
};


void InitPlayer(Player* player) {
	player->x = 100;
	player->y = 100;
};
void PositionPlayer(Player* player) {

	al_draw_filled_rectangle(player->x, player->y, player->x + 30, player->y + 30, al_map_rgb(255, 255, 0));
	return;
};


void InitContainer(Container* container, Background* background, Player* player)
{
	//Inicia o alegro
	if (!al_init())
		LogError("Falha ao carregar o allegro");

	//Criacao do Display
	if (!(container->window = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT)))
		LogError("Falha ao carregar janela");


	// Add-on Allegro
	al_init_ttf_addon();
	al_init_font_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_init_primitives_addon();


	srand(time(NULL));

	// Carrega Background
	InitBackground(background, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_load_bitmap("src/imgs/oficce.bmp"));
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
	background->y = y + 120;
	background->width = width;
	background->height = height;
	background->image = image;

	for(int i = 0; i < 2; i ++)
	{
		background->cloud[i].speed = 15;
		background->cloud[i].active = false;
	}

	if (!background->image)
		LogError("Falha ao carregar a imagem");
}
void DrawBackground(Background* background)
{
	al_draw_bitmap(background->image, background->x, background->y, 0);


	if (true) {
		al_draw_filled_rectangle(0, 120, WINDOW_WIDTH, 0, al_map_rgb(65, 166, 246));

		MovingCloudBackground(&background->cloud);
		DrawCloudBackground(&background->cloud);
	}
	else {
		fadeInNight(background, 15);
	}

		
	if (background->x + background->width < WINDOW_WIDTH)
		al_draw_bitmap(background->image, background->x + background->width, background->y, 0);
}


void MovingCloudBackground(Cloud cloud[])
{
	for (int i = 0; i < 2; i++) 
	{
		if (!cloud[i].active)
		{
			cloud[i].active = true;
			cloud[i].x = WINDOW_WIDTH;
			cloud[i].y = rand() % 5;

			if (rand() % 2 == 0)
				cloud[i].image = al_load_bitmap("src/imgs/cloud.png");
			else
				cloud[i].image = al_load_bitmap("src/imgs/cloud2.png");
				
			break;	
		}	
	}
	return;
}
void attCloudPosition(Cloud cloud[]) {
	for (int i = 0; i < 2; i++)
	{
		if (cloud[i].active)
		{
			cloud[i].x -= cloud[i].speed;

			if (cloud[i].x < -500)
			{
				cloud[i].active = false;
				al_destroy_bitmap(cloud[i].image);
			}
;		}
	}
	return;
}
void DrawCloudBackground(Cloud cloud[]) {
	for (int i = 0; i < 2; i++)
	{
		if (cloud[i].active)
			al_draw_bitmap(cloud[i].image, cloud[i].x, cloud[i].y, 0);
	}
}

void fadeInNight(Background* background, float speed) {

	if (speed < 0)
		speed = 1;
	

	int alfa;
	for (alfa = 0; alfa <= 255; alfa += speed)
	{
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_tinted_bitmap(background->image, al_map_rgba(alfa, alfa, alfa, alfa), background->x, background->y, 0);
		al_flip_display();
		al_rest(0.005);
	}
	return;
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
	ALLEGRO_FONT* fontFps = al_load_font("src/font/BAVEUSE.TTF", 20, NULL);
	ALLEGRO_FONT* fontHora = al_load_font("src/font/minecraft.TTF", 20, NULL);

	while (!container->hasFinished)
	{
		DrawBackground(background);
		PositionPlayer(player);
		LogFrames(frame, fontFps);

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

		
		if ((al_get_time() - startTime) < 1.0 / FPS)
			al_rest((1.0 / FPS) - (al_get_time() - startTime));
		
		else if (frame > 60)
			frame--;

		frame++;
		DrawCloudBackground(background->cloud);
		MovingCloudBackground(background->cloud);

		attCloudPosition(background->cloud);

	}

	EndContainer(container);
}