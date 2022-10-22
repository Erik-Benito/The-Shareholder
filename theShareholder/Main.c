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
void LogFrames(int fps);

void InitContainer(Container* container, Background* background, Player* player, Mouse* mouse);
void EndContainer(Container* container);

void InitBackground(Background* background, float x, float y, int width, int height, ALLEGRO_BITMAP* image);
void DrawBackground(Background* background);

void InitPlayer(Player* player);
void PositionPlayer(Player* player, ALLEGRO_BITMAP* spritePlayer, int curFrame);

void InitEvent(Container* container, Background* background, Player* player, Mouse* mouse);
void ControlEvent(Container* container, Background* background, Player* player, Mouse* mouse);

void PositionMouse(Mouse* mouse, int x, int y);

void PlayerMoveUp(Player* player);
void PlayerMoveDown(Player* player);
void PlayerMoveRIGHT(Player* player);
void PlayerMoveLEFT(Player* player);

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
	al_draw_textf(font, al_map_rgb(255, 255, 255), 50, 10, ALLEGRO_ALIGN_CENTRE, "FPS: %d", fps);
}


void InitPlayer(Player* player) {
	player->x = 100;
	player->y = 100;
};
void PositionPlayer(Player* player, ALLEGRO_BITMAP* spritePlayer, int curFrame) {
	al_draw_bitmap_region(spritePlayer, 0 , 0, 30, 32, player->x, player->y, NULL);
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
	//instala as parada do mouse
	al_install_mouse();
	al_init_primitives_addon();

	// Carrega Background
	InitBackground(background, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_load_bitmap("teste1.bmp"));
	DrawBackground(background);

	// Criando Personagem
	InitPlayer(player);

	// Carregando Eventos do game 
	InitEvent(container, background, player, mouse);

};
void EndContainer(Container* container, ALLEGRO_BITMAP* spritePlayer) {
	
	al_destroy_display(container->window);
	al_destroy_bitmap(spritePlayer);
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

	al_register_event_source(container->eventQueue, al_get_keyboard_event_source());
	al_register_event_source(container->eventQueue, al_get_mouse_event_source());
	al_register_event_source(container->eventQueue, al_get_timer_event_source(timer));
	al_start_timer(timer);
	ControlEvent(container, background, player, mouse);
}


void ControlEvent(Container* container, Background* background, Player* player, Mouse* mouse)
{
	int frame = 0;
	double startTime = al_get_time();
	ALLEGRO_FONT* font = al_load_font("BAVEUSE.TTF", 20, NULL);
	al_hide_mouse_cursor(container->window);
	ALLEGRO_BITMAP* spritePlayer = al_load_bitmap("Walk2(teste2001).png");
	int dir = DOWN, SourceX = 0, sourceY = 0;
	player->mvSpeed = 10;
	int curFrame = 0;
	bool redraw = true, active = false;

	while (!container->hasFinished)
	{
		DrawBackground(background);
		
		PositionMouse(mouse);
		LogFrames(frame, font);
		al_flip_display();
		PositionPlayer(player, spritePlayer, curFrame);
		ALLEGRO_EVENT event;
		al_wait_for_event(container->eventQueue, &event);
		ALLEGRO_KEYBOARD_STATE keyState;

		if (event.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
			active = true;
			if (keys[UP])
				PlayerMoveUp(player);
			if (keys[DOWN])
				PlayerMoveDown(player);
			if (keys[LEFT])
				PlayerMoveLeft(player);
			if (keys[RIGHT])
				PlayerMoveRight(player);
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				container->hasFinished = true;

			switch (event.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_LEFT:
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
		} else 
			//active = false;

		//if (active) {

		//}

		if (redraw && al_is_event_queue_empty(container->eventQueue)) 
		{
			redraw = false;
			
			PositionPlayer(player, spritePlayer, curFrame);
			DrawBackground(background);
		}
			
		if (++frame >= 60) 
		{
			frame = 0;
		}
	}

	EndContainer(container, spritePlayer);
}