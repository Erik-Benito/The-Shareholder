#include <stdio.h>

#include <allegro5\allegro.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>


#include "game.h"
#include "mouse.h"
#include "spritesheet.h"

Player player;
Container container;
Background background;
Mouse mouse;


// Prototypes
void LogError(char* error);
void LogFrames(int fps, ALLEGRO_FONT* font);

void InitContainer(Container* container, Background* background, Player* player, Mouse* mouse);
void EndContainer(Container* container, Player* player);

void InitBackground(Background* background, float x, float y, int width, int height, ALLEGRO_BITMAP* image);
void DrawBackground(Background* background);

void InitPlayer(Player* player);
void PositionPlayer(Player* player);

void InitEvent(Container* container, Background* background, Player* player, Mouse* mouse);
void ControlEvent(Container* container, Background* background, Player* player, Mouse* mouse);

void PositionMouse(Mouse* mouse);

void PlayerMoveUp(Player* player);
void PlayerMoveDown(Player* player);
void PlayerMoveRight(Player* player);
void PlayerMoveLeft(Player* player);



int main(void) {
	InitContainer(&container, &background, &player, &mouse);
	return 0;
};



void LogError(char* error)
{
	al_show_native_message_box(NULL, "Aviso!", "ERROR:", *error, NULL, ALLEGRO_MESSAGEBOX_ERROR);
	return;
};
void LogFrames(int fps, ALLEGRO_FONT* font)
{
	al_draw_textf(font, al_map_rgb(0, 0, 0), 100, 100, ALLEGRO_ALIGN_CENTRE, "FPS: %d", fps);
}


void InitPlayer(Player* player) {
	player->x = 100;
	player->y = 100;
	player->mvSpeed = 10;
	player->spritePlayer = al_load_bitmap("PERSONAGEM.png");
};
void PositionPlayer(Player* player) {
	al_draw_bitmap_region(player->spritePlayer, 0, 0, 30, 32, player->x, player->y, NULL);
	return;
}


void PlayerMoveUp(Player* player) {
	player->y -= player->mvSpeed;
}
void PlayerMoveDown(Player* player) {
	player->y += player->mvSpeed;
}
void PlayerMoveRight(Player* player) {
	player->x += player->mvSpeed;
}
void PlayerMoveLeft(Player* player) {
	player->x -= player->mvSpeed;
}


void PositionMouse(Mouse* mouse) {

	al_draw_filled_rectangle(mouse->x, mouse->y, mouse->x + 10, mouse->y + 10, al_map_rgb(44, 117, 255));
	return;
}


void InitContainer(Container* container, Background* background, Player* player, Mouse* mouse)
{
	//Inicia o alegro
	if (!al_init())
		LogError("Falha ao carregar o allegro");

	//Criacao do Display
	if (!(container->window = al_create_display(WINDOW_HEIGHT, WINDOW_WIDTH)))
		LogError("Falha ao carregar janela");

	if (!al_set_system_mouse_cursor(container->window, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
		LogError("Falha ao carregar o cursor");

	// Add-on Allegro
	al_init_ttf_addon();
	al_init_font_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_init_primitives_addon();

	//instala as parada do mouse
	al_install_mouse();
	al_hide_mouse_cursor(container->window);


	// Carrega Background
	InitBackground(background, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_load_bitmap("oficce.bmp"));
	DrawBackground(background);

	// Criando Personagem
	InitPlayer(player);

	// Carregando Eventos do game 
	InitEvent(container, background, player, mouse);

};
void EndContainer(Container* container, Player* player) {

	al_destroy_display(container->window);
	al_destroy_bitmap(player->spritePlayer);
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


void InitEvent(Container* container, Background* background, Player* player, Mouse* mouse)
{
	ALLEGRO_TIMER* timer = NULL;
	timer = al_create_timer(1.0 / FPS);

	container->hasFinished = false;
	container->needRedraw = true;

	container->eventQueue = NULL;
	container->eventQueue = al_create_event_queue();

	if (!container->eventQueue)
		LogError("Nã0 foi possivel carregar o evento");

	al_register_event_source(container->eventQueue, al_get_mouse_event_source());
	al_register_event_source(container->eventQueue, al_get_keyboard_event_source());

	ControlEvent(container, background, player, mouse);
}


void ControlEvent(Container* container, Background* background, Player* player, Mouse* mouse)
{

	ALLEGRO_FONT* font = al_load_font("BAVEUSE.TTF", 20, NULL);
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

	int dir = DOWN, frame = 0, sourceX = 0, sourceY = 0;

	bool redraw = false, active = false, positionPlayer = player->x;

	al_register_event_source(container->eventQueue, al_get_timer_event_source(timer));

	al_start_timer(timer);

	while (!container->hasFinished)
	{
		ALLEGRO_EVENT event;
		ALLEGRO_KEYBOARD_STATE keyState;

		al_wait_for_event(container->eventQueue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {
			if (keys[UP])
				PlayerMoveUp(player);
			if (keys[DOWN])
				PlayerMoveDown(player);
			if (keys[LEFT])
				PlayerMoveLeft(player);
			if (keys[RIGHT])
				PlayerMoveRight(player);

			active = true;
			redraw = true;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				container->hasFinished = true;

			switch (event.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				dir = UP;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				dir = DOWN;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				dir = RIGHT;
				break;
			case ALLEGRO_KEY_LEFT:
				dir = LEFT;
				keys[LEFT] = true;
				break;
			}
		}
		else if (event.type == ALLEGRO_EVENT_KEY_UP)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				container->hasFinished = true;

			switch (event.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			}
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mouse->x = event.mouse.x;
			mouse->y = event.mouse.y;
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (event.mouse.button & 1) { //botão esquerdo

			}
			if (event.mouse.button & 2) { // botão direito

			}
		}

		if (active)
			sourceX += al_get_bitmap_width(player->spritePlayer)/4;
		else
			sourceX = 100;
		

		if (sourceX >= al_get_bitmap_width(player->spritePlayer))
			sourceX = 0;

		if (redraw)
		{
			redraw = false;

			LogFrames(frame, font);

			al_draw_bitmap_region(player->spritePlayer, sourceX, dir * al_get_bitmap_height(player->spritePlayer) / 4, 100, 140, player->x, player->y, NULL);
			PositionMouse(mouse);


			al_flip_display();
			DrawBackground(background);
		}
				

	}

	EndContainer(container, player);
}