#include <stdio.h>
#include <time.h>
#include <math.h>

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
TimerGame timeGame;
Mouse mouse;
Wallet wallet;


// Prototypes
void LogError(char* error);
void LogFrames(int fps, ALLEGRO_FONT* font);
void LogWallet(ALLEGRO_FONT* font, Wallet* wallet);

void InvestmentReturn(Wallet* wallet);
void AddAmount(Wallet* wallet);
void RemoveBalance(Wallet* wallet);

void InitContainer(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame);
void EndContainer(Container* container, Player* player);

void InitWallet(Wallet* wallet);

void InitBackground(Background* background, float x, float y, int width, int height, ALLEGRO_BITMAP* image);
void DrawBackground(Background* background, TimerGame* timerGame);

// Gera o inicio das nuvens
void InitMovingCloudBackground(Cloud cloud[]);
// Passa as nuvens pelo jogo
void attCloudPosition(Cloud cloud[]);
// Desenha as nuvens no bg
void DrawCloudBackground(Cloud cloud[]);
// Efeito de escuro
void fadeInNight(Background* background, float speed);

void InitTimerGame(TimerGame *timerGame);
void attTimerGame(TimerGame *timerGame, int hours, int minutes, int seconds, int days);

void InitPlayer(Player* player);
void DrawPlayer(Player* player);

void InitEvent(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame);
void ControlEvent(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame);

void DrawMouse(Mouse* mouse);

void PlayerMoveUp(Player* player);
void PlayerMoveDown(Player* player);
void PlayerMoveRight(Player* player);
void PlayerMoveLeft(Player* player);



int main(void) {
	InitContainer(&container, &background, &player, &mouse, &timeGame, &wallet);
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
void LogWallet(ALLEGRO_FONT* font, Wallet* wallet)
{
	float balance = wallet->amount + wallet->investedProfit + wallet->lossPerDay;
	al_draw_textf(font, al_map_rgb(0, 0, 0), 70, 150, ALLEGRO_ALIGN_CENTER, "%0.2f", balance);
	return;
};


void AddAmount(Wallet* wallet) {
	wallet->amount+=50;
}
void InvestmentReturn(Wallet* wallet)
{
	wallet->investedProfit+= 300;
}
void RemoveBalance(Wallet* wallet)
{
	wallet->lossPerDay -= 100;
}


void InitWallet(Wallet *wallet) {
	wallet->amount = 100;
	wallet->investedProfit = 0;
	wallet->lossPerDay = 0;
}
void InitTimerGame(TimerGame *timerGame)
{
	timerGame->hours = 3.7494;
	timerGame->minutes = 0;
	timerGame->seconds = 0;

	timerGame->hoursTotal = 0;
	timerGame->minutesTotal = 0;
	timerGame->secondsTotal = 0;
	timerGame->days = 0;

}
void attTimerGame(TimerGame *timerGame, int hours, int minutes, int seconds, int days) 
{
	timerGame->hoursTotal += hours;
	timerGame->minutesTotal += minutes;
	timerGame->secondsTotal += seconds;
	timerGame->days += days;
}


void InitPlayer(Player* player) {
	
	player->x = 100;
	player->y = 100;
	
	player->speed = 10;
	
	player->sourceX = 100;
	player->sourceY = 0;
	player->needRedraw = true;

	player->spritePlayer = al_load_bitmap("src/imgs/player.png");
};
void DrawPlayer(Player* player) {
	al_draw_bitmap_region(player->spritePlayer, player->sourceX, player->sourceY, 100, 140, player->x, player->y, NULL);
	return;
}


void PlayerMoveUp(Player* player) {
	player->y -= player->speed;
}
void PlayerMoveDown(Player* player) {
	player->y += player->speed;
}
void PlayerMoveRight(Player* player) {
	player->x += player->speed;
}
void PlayerMoveLeft(Player* player) {
	player->x -= player->speed;
}


void DrawMouse(Mouse* mouse) {

	al_draw_filled_rectangle(mouse->x, mouse->y, mouse->x + 10, mouse->y + 10, al_map_rgb(44, 117, 255));
	return;
};


void InitContainer(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Wallet* wallet)
{
	//Inicia o alegro
	if (!al_init())
		LogError("Falha ao carregar o allegro");

	//Criacao do Display
	if (!(container->window = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT)))
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
	InitBackground(background, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_load_bitmap("src/imgs/oficce.bmp"));
	DrawBackground(background,timerGame);

	// Criando Personagem
	InitPlayer(player);

	// Carrega tempo de jogo
	InitTimerGame(timerGame);

	// Carrega a carteira do Player
	InitWallet(wallet);

	// Carregando Eventos do game 
	InitEvent(container, background, player, mouse, timerGame, wallet);

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
	background->y = y + 120;
	background->width = width;
	background->height = height;
	background->image = image;

	for(int i = 0; i < 5; i ++)
	{
		background->cloud[i].speed = 15;
		background->cloud[i].active = false;
	}

	if (!background->image)
		LogError("Falha ao carregar a imagem");
}
void DrawBackground(Background* background, TimerGame*  timerGame)
{
	al_draw_bitmap(background->image, background->x, background->y, 0);

	printf("Horas:%f Min: %f\n", timerGame->minutes, timerGame->hours);
	if (lround(timerGame->hours * 4.8) < 19 && lround(timerGame->hours * 4.8) > 6) {
		al_draw_filled_rectangle(0, 120, WINDOW_WIDTH, 0, al_map_rgb(65, 166, 246));

		InitMovingCloudBackground(&background->cloud);
		DrawCloudBackground(&background->cloud);
	}
	else if (lround(timerGame->hours) == 4 && timerGame->minutes == 0 && timerGame->seconds == 0)
	{	
		fadeInNight(background, 1000);
		al_draw_filled_rectangle(0, 120, WINDOW_WIDTH, 0, al_map_rgb(0, 0, 0));
		
	}
	else 
	{
		al_draw_filled_rectangle(0, 120, WINDOW_WIDTH, 0, al_map_rgb(0, 0, 0));
	}


		
	if (background->x + background->width < WINDOW_WIDTH)
		al_draw_bitmap(background->image, background->x + background->width, background->y, 0);
}


void InitMovingCloudBackground(Cloud cloud[])
{
	for (int i = 0; i < 5; i++) 
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
	for (int i = 0; i < 5; i++)
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
	for (int i = 0; i < 5; i++)
	{
		if (cloud[i].active)
			al_draw_bitmap(cloud[i].image, cloud[i].x, cloud[i].y, 0);
	}
}

void fadeInNight(Background* background, float speed) {

	if (speed < 0)
		speed = 1;
	

	int alfa;
	for (alfa = 0; alfa <= 255; alfa += 15)
	{
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_tinted_bitmap(background->image, al_map_rgba(alfa, alfa, alfa, alfa), background->x, background->y, 0);
		al_flip_display();
		al_rest(0.05);
	}
	return;
}


void InitEvent(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Wallet* wallet)
{
	ALLEGRO_TIMER* timer = NULL;
	timer = al_create_timer(1.0 / FPS);

	container->hasFinished = false;
	container->needRedraw = false;

	container->eventQueue = NULL;
	container->eventQueue = al_create_event_queue();

	if (!container->eventQueue)
		LogError("Nã0 foi possivel carregar o evento");

	al_register_event_source(container->eventQueue, al_get_mouse_event_source());
	al_register_event_source(container->eventQueue, al_get_keyboard_event_source());

	ControlEvent(container, background, player, mouse, timerGame, wallet);
}
void ControlEvent(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Wallet* wallet)
{

	ALLEGRO_FONT* font = al_load_font("src/font/BAVEUSE.TTF", 20, NULL);
	ALLEGRO_FONT* fontTimer = al_load_font("src/font/MINECRAFT.TTF", 20, NULL);
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
	

	int dir = DOWN, frame = 0;

	al_register_event_source(container->eventQueue, al_get_timer_event_source(timer));

	al_start_timer(timer);

	while (!container->hasFinished)
	{
		ALLEGRO_EVENT event;
		ALLEGRO_KEYBOARD_STATE keyState;

		al_wait_for_event(container->eventQueue, &event);
		LogWallet(fontTimer, wallet);

		if (event.type == ALLEGRO_EVENT_TIMER) {
			player->needRedraw = true;
			if (keys[UP])
				PlayerMoveUp(player);
			else if (keys[DOWN])
				PlayerMoveDown(player);
			else if (keys[LEFT])
				PlayerMoveLeft(player);
			else if (keys[RIGHT])
				PlayerMoveRight(player);
			else 
				player->needRedraw = false;
			
			timerGame->seconds+=0.0005786;
			if (timerGame->seconds >= 0.034716)
			{
				AddAmount(wallet);
				timerGame->seconds = 0;
				timerGame->minutes += 0.034716;
				
				

				if (timerGame->minutes >= 0.2083)
				{
					timerGame->hours+= 0.2083;
					timerGame->minutes = 0;
					RemoveBalance(wallet);

					if (timerGame->hours >= 4.9992)
					{
						timerGame->hours = 0;
						attTimerGame(timerGame, 1, 60, 3600, 1);
						
					}
				}
			}


			container->needRedraw = true;
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
				InvestmentReturn(wallet);
			}
			if (event.mouse.button & 2) { // botão direito

			}
		}

		if (player->needRedraw)
			player->sourceX += al_get_bitmap_width(player->spritePlayer)/4;
		else
			player->sourceX = 100;
		

		if (player->sourceX >= al_get_bitmap_width(player->spritePlayer))
			player->sourceX = 0;

		player->sourceY = dir * al_get_bitmap_height(player->spritePlayer) / 4;

		if (container->needRedraw)
		{
			if (frame >= FPS)
				frame--;
			else
				frame++;

			container->needRedraw = false;

			al_draw_textf(fontTimer, al_map_rgb(255, 0, 0), 500, 140, ALLEGRO_ALIGN_CENTRE, "%d : %d0",lround(timerGame->hours*4.8) , lround(timerGame->minutes * 28));
			DrawPlayer(player);
			DrawMouse(mouse);


			al_flip_display();
			DrawBackground(background,timerGame);
			DrawCloudBackground(background->cloud);
			InitMovingCloudBackground(background->cloud);

			attCloudPosition(background->cloud);
			
		}
		
		

		

	}

	EndContainer(container, player);
}