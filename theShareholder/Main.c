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
#include "collisions.h"

Player player;
Container container;
Background background;
TimerGame timeGame;
Mouse mouse;
Hitbox hitboxs[4];

// Prototypes
void LogError(char* error);
void LogFrames(int fps, ALLEGRO_FONT* font, ALLEGRO_COLOR* COR);
void LogHours(float hours, float minutes, int x, int y, ALLEGRO_FONT* font, ALLEGRO_COLOR cor);
void ShowMoney(int x, int y, int value, ALLEGRO_FONT* font);

void InitContainer(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Hitbox hitboxs[5]);
void EndContainer(Container* container, Player* player);

void InitBackground(Background* background, float x, float y, int width, int height, ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* imagePc);
void DrawBackground(Background* background, TimerGame* timerGame, Container* container);

// Gera o inicio das nuvens
void InitMovingCloudBackground(Cloud cloud[]);
// Passa as nuvens pelo jogo
void attCloudPosition(Cloud cloud[]);
// Desenha as nuvens no bg
void DrawCloudBackground(Cloud cloud[]);
// Efeito de escuro
void fadeInNight(Background* background, float speed);
// Tempo dentro do Jogo
void InitTimerGame(TimerGame *timerGame);
void attTimerGame(TimerGame *timerGame, int hours, int minutes, int seconds, int days);
// Inicia o Jogador
void InitPlayer(Player* player);
void DrawPlayer(Player* player);
//Linha de Eventos
void InitEvent(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Hitbox hitboxs[5]);
void ControlEvent(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Hitbox hitboxs[5]);
// Inicia o Mouse
void DrawMouse(Mouse* mouse);
//Movimentos do Jogador
void PlayerMoveUp(Player* player);
void PlayerMoveDown(Player* player);
void PlayerMoveRight(Player* player);
void PlayerMoveLeft(Player* player);
//Colisões
void InitCollision(Player* player, Hitbox hitboxs[5]);
bool HasCollision(Player* player, Hitbox hitboxs[5]);

//=================================//

int main(void) {
	InitContainer(&container, &background, &player, &mouse, &timeGame, &hitboxs);
	return 0;
};


//=================================//


void LogError(char* error)
{
	al_show_native_message_box(NULL, "Aviso!", "ERROR:", error, NULL, ALLEGRO_MESSAGEBOX_ERROR);
	return;
};
void LogFrames(int fps, ALLEGRO_FONT* font, ALLEGRO_COLOR* COR)
{
	al_draw_textf(font, al_map_rgb(0, 0, 0), 50, 10, ALLEGRO_ALIGN_CENTRE, "FPS: %d", fps);
};
void LogHours(float hours, float minutes, int x, int y, ALLEGRO_FONT *font, ALLEGRO_COLOR cor) {
	al_draw_textf(font, cor, x, y, ALLEGRO_ALIGN_CENTRE, "%d : %d0", lround(hours * 4.8), lround(minutes * 28));
	return;
}
void ShowMoney(int x, int y, int value, ALLEGRO_FONT* font) {
	al_draw_textf(font, al_map_rgb(255, 255, 255), x, y, ALLEGRO_ALIGN_CENTRE, "RS:%d", value);
}

void InitTimerGame(TimerGame *timerGame)
{
	timerGame->hours = 0;
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
	
	player->x = 430;
	player->y = 50 + 120;
	
	player->speed = 10;
	
	player->sourceX = 0;
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


void initMouse(Mouse* mouse) {

	mouse->image = NULL;

	mouse->x = 250;
	mouse->y = 250;

	mouse->image = al_load_bitmap("src/imgs/mouse.png");

	if (!mouse->image)
		LogError("Falha ao carregar a imagem do mouse");
	
	return;
}
void DrawMouse(Mouse* mouse) {
	al_draw_bitmap(mouse->image, mouse->x, mouse->y, 0);
	return;
};

void InitCollision(Player* player, Hitbox hitboxs[5]) {

	// Player
	hitboxs[4].initX = 10;
	hitboxs[4].endX  = 80;
	hitboxs[4].initY = 110;
	hitboxs[4].endY  = 140;

	// ilha de mesa no centro da sala
	hitboxs[0].initX = 150;
	hitboxs[0].initY = 250 + 120;
	hitboxs[0].endX = 420;
	hitboxs[0].endY = 480 + 120;
	
	// Mesa encostada na parede
	hitboxs[1].initX = 580;
	hitboxs[1].initY = 240 + 120;
	hitboxs[1].endX = 730;
	hitboxs[1].endY = 640 + 120;
	
	// Mesa do chefe
	hitboxs[2].initX = 820;
	hitboxs[2].initY = 320 + 120;
	hitboxs[2].endX = 870;
	hitboxs[2].endY = 640 + 120;
		
	// limites do mapa
	hitboxs[3].initX = -10;
	hitboxs[3].initY = 150 + 120;
	hitboxs[3].endX = 1000;
	hitboxs[3].endY = 550 + 120;

}


bool HasCollision (Player* player, Hitbox hitboxs[5]) {

	// Plaver
	hitboxs[4].initX += 10;
	hitboxs[4].endX += 80;
	hitboxs[4].initY += 110;
	hitboxs[4].endY += 140;

	bool HasCollisionResponse = false;

	if		(hitboxs[4].endX > hitboxs[0].initX  && hitboxs[4].initX < hitboxs[0].endX && hitboxs[4].endY > hitboxs[0].initY && hitboxs[4].initY < hitboxs[0].endY)
		HasCollisionResponse = true;
	else if (hitboxs[4].endX > hitboxs[1].initX  && hitboxs[4].initX < hitboxs[1].endX && hitboxs[4].endY > hitboxs[1].initY && hitboxs[4].initY < hitboxs[1].endY) 
		HasCollisionResponse = true;
	else if (hitboxs[4].endX > hitboxs[2].initX  && hitboxs[4].initX < hitboxs[2].endX && hitboxs[4].endY > hitboxs[2].initY && hitboxs[4].initY < hitboxs[2].endY) 
		HasCollisionResponse = true;
	else if (hitboxs[4].initX < hitboxs[3].initX || hitboxs[4].endX  > hitboxs[3].endX) 
		HasCollisionResponse = true;
	else if (hitboxs[4].initY < hitboxs[3].initY || hitboxs[4].endY  > hitboxs[3].endY) 
		HasCollisionResponse = true;
	
	return HasCollisionResponse;
}

void InitContainer(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Hitbox hitboxs[5])
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
	InitBackground(background, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_load_bitmap("src/imgs/oficce.bmp"), al_load_bitmap("src/imgs/pcMode.png"));
	DrawBackground(background,timerGame, container);

	// Criando Personagem
	InitPlayer(player);

	// Carrega tempo de jogo
	InitTimerGame(timerGame);

	// Criando Mouse
	initMouse(mouse);

	// Carregando Eventos do game 
	InitEvent(container, background, player, mouse, timerGame, hitboxs);

};
void EndContainer(Container* container, Player* player) {

	al_destroy_display(container->window);
	al_destroy_bitmap(player->spritePlayer);
	al_destroy_event_queue(container->eventQueue);
}


void InitBackground(Background* background, float x, float y, int width, int height, ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* imagePc)
{
	background->image = NULL;
	background->pcBackground.image = NULL;

	background->x = x;
	background->y = y ;
	background->image = image;
	background->width = width;
	background->height = height;

	background->pcBackground.x = x;
	background->pcBackground.y = y;
	background->pcBackground.width = width;
	background->pcBackground.height = height;
	background->pcBackground.image = imagePc;
	
	for(int i = 0; i < 5; i ++)
	{
		background->cloud[i].speed = 15;
		background->cloud[i].active = false;
	}

	if (!background->pcBackground.image)
		LogError("Falha ao Carregar a imagem do computador");

	if (!background->image)
		LogError("Falha ao carregar a imagem do escritorio");
}
void DrawBackground(Background* background, TimerGame*  timerGame, Container* container)
{

	if (!container->isPcMode)
	{
		al_draw_bitmap(background->image, background->x + 120, background->y + 120, 0);
		
		if (lround(timerGame->hours * 4.8) < 19 && lround(timerGame->hours * 4.8) > 6) {
			al_draw_filled_rectangle(0, 120, WINDOW_WIDTH, 0, al_map_rgb(65, 166, 246));

			InitMovingCloudBackground(&background->cloud);
			DrawCloudBackground(&background->cloud);
		}
		else if (timerGame->hoursTotal == 3.9577)
		{
			fadeInNight(background, 1000);
			al_draw_filled_rectangle(0, 120, WINDOW_WIDTH, 0, al_map_rgb(0, 0, 0));
		}
		else
			al_draw_filled_rectangle(0, 120, WINDOW_WIDTH, 0, al_map_rgb(0, 0, 0));
	}
	else 
		al_draw_bitmap(background->pcBackground.image, background->x, background->y, 0);
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
	}
	return;
}


void InitEvent(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Hitbox hitboxs[5])
{
	ALLEGRO_TIMER* timer = NULL;
	timer = al_create_timer(1.0 / FPS);

	container->hasFinished = false;
	container->needRedraw = false;
	container->isPcMode = true;

	container->eventQueue = NULL;
	container->eventQueue = al_create_event_queue();

	if (!container->eventQueue)
		LogError("Nã0 foi possivel carregar o evento");

	al_register_event_source(container->eventQueue, al_get_mouse_event_source());
	al_register_event_source(container->eventQueue, al_get_keyboard_event_source());

	InitCollision(player, hitboxs);
	ControlEvent(container, background, player, mouse, timerGame, hitboxs);
}
void ControlEvent(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Hitbox hitboxs[5])
{

	ALLEGRO_FONT* font = al_load_font("src/font/BAVEUSE.TTF", 20, NULL);
	ALLEGRO_FONT* fontInvest = al_load_font("src/font/InvestFont.ttf", 12, NULL);
	ALLEGRO_FONT* fontTimer = al_load_font("src/font/MINECRAFT.TTF", 20, NULL);
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

	int dir = DOWN, frame = 0, investSafeForValue = 5, investDangerForValue = 0;

	al_register_event_source(container->eventQueue, al_get_timer_event_source(timer));

	al_start_timer(timer);

	while (!container->hasFinished)
	{
		ALLEGRO_EVENT event;
		ALLEGRO_KEYBOARD_STATE keyState;

		al_wait_for_event(container->eventQueue, &event); 

		if (event.type == ALLEGRO_EVENT_TIMER) 
		{
			player->needRedraw = true;
			if (keys[UP])
			{
				hitboxs[4].initY = player->y - player->speed;
				hitboxs[4].endY = player->y - player->speed;
				hitboxs[4].initX = player->x;
				hitboxs[4].endX = player->x;

				if (!HasCollision(player, hitboxs)) 
					PlayerMoveUp(player);
			}
			else if (keys[DOWN]){
				hitboxs[4].initY = player->y + player->speed;
				hitboxs[4].endY = player->y + player->speed;
				hitboxs[4].initX = player->x;
				hitboxs[4].endX = player->x;

				if (!HasCollision(player, hitboxs)) 
					PlayerMoveDown(player);
			}
			else if (keys[LEFT]){
				hitboxs[4].initY = player->y;
				hitboxs[4].endY = player->y;
				hitboxs[4].initX = player->x - player->speed;
				hitboxs[4].endX = player->x - player->speed;
				
				if (!HasCollision(player, hitboxs))
					PlayerMoveLeft(player);
			}
			else if (keys[RIGHT]){
				hitboxs[4].initY = player->y;
				hitboxs[4].endY = player->y;
				hitboxs[4].initX = player->x + player->speed;
				hitboxs[4].endX = player->x + player->speed;

				if (!HasCollision(player, hitboxs))
					PlayerMoveRight(player);
			}
			else 
				player->needRedraw = false;
			
			// 1 para 1
			// seconds = 0,0005786
			// minutes = 0,034716
			// hours   = 4,9992
			
			timerGame->seconds+=0.0005786;
			if (timerGame->seconds >= 0.034716)
			{
				timerGame->seconds = 0;

				timerGame->minutes += 0.034716;
				
				if (timerGame->minutes >= 0.2083)
				{
					timerGame->hours+= 0.2083;
					timerGame->minutes = 0;
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

			printf("x:%d   |   y:%d\n", mouse->x, mouse->y);
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (event.mouse.button & 1) { //botão esquerdo
				if (mouse->x >= 305 && mouse->x <= 460 && mouse->y >= 205 && mouse->y <= 360)
					investSafeForValue *= 2;
			}
			if (event.mouse.button & 2) { // botão direito

			}
		}

		if (player->needRedraw)
			player->sourceX += al_get_bitmap_width(player->spritePlayer)/4;
		else
			player->sourceX = 0;
		

		if (player->sourceX >= al_get_bitmap_width(player->spritePlayer))
			player->sourceX = 0;

		player->sourceY = dir * al_get_bitmap_height(player->spritePlayer) / 4;

		if (container->needRedraw && !container->isPcMode)
		{
			container->needRedraw = false;

			DrawPlayer(player);
			DrawMouse(mouse);

			LogHours(timerGame->hours, timerGame->minutes, 500, 140, fontTimer, al_map_rgb(255, 0 ,0));

			al_flip_display();

			DrawBackground(background,timerGame, container);

			DrawCloudBackground(background->cloud);
			InitMovingCloudBackground(background->cloud);
			attCloudPosition(background->cloud);

		}
		else if (container->needRedraw && container->isPcMode)
		{

			container->needRedraw = false;

			LogHours(timerGame->hours, timerGame->minutes, 960, 625, fontTimer, al_map_rgb(0, 0, 0));
			DrawMouse(mouse);
			
			// Exibi os valores de dinheito

			// valor investido
			ShowMoney(415, 358, investSafeForValue, fontInvest);

			// valor para investir
			ShowMoney(415, 445, 150, fontInvest);


			al_flip_display();

			DrawBackground(background, timerGame, container);
		}
		
		if (frame >= FPS)
			frame--;
		else
			frame++;

	}

	EndContainer(container, player);
}