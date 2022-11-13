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
void LogFrames(int fps, ALLEGRO_FONT* font, ALLEGRO_COLOR* COR);
void LogHours(float hours, float minutes, int x, int y, ALLEGRO_FONT* font, ALLEGRO_COLOR cor);
void ShowMoney(int x, int y, int value, ALLEGRO_FONT* font);
void LogQtyInvestCompany(int x, int y, int value, ALLEGRO_FONT* font);
void LogWallet(ALLEGRO_FONT* font, Wallet* wallet);

void InvestmentReturn(Wallet* wallet);
void AddAmount(Wallet* wallet);
void RemoveBalance(Wallet* wallet);

void InitContainer(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Wallet* wallet);
void EndContainer(Container* container, Player* player);

void InitWallet(Wallet* wallet);
void statusProgress(Wallet* wallet, int x, int y);

void InitBackground(Background* background, float x, float y, int width, int height, ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* imagePc);
void DrawBackground(Background* background, TimerGame* timerGame, Container* container);

void InitMovingCloudBackground(Cloud cloud[]);
void attCloudPosition(Cloud cloud[]);
void DrawCloudBackground(Cloud cloud[]);
void fadeInNight(Background* background, float speed);

void InitTimerGame(TimerGame *timerGame);
void attTimerGame(TimerGame *timerGame, int hours, int minutes, int seconds, int days);

void InitPlayer(Player* player);
void DrawPlayer(Player* player);

void InitEvent(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Wallet* wallet);
void ControlEvent(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Wallet* wallet);

void initMouse(Mouse* mouse);
void DrawMouse(Mouse* mouse);

void PlayerMoveUp(Player* player);
void PlayerMoveDown(Player* player);
void PlayerMoveRight(Player* player);
void PlayerMoveLeft(Player* player);

//=================================//


int main(void) {
	InitContainer(&container, &background, &player, &mouse, &timeGame, &wallet);
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
	return;
};
void LogWallet(ALLEGRO_FONT* font, Wallet* wallet)
{
	al_draw_textf(font, al_map_rgb(0, 0, 0), 250, 250, ALLEGRO_ALIGN_CENTER, "RS:%d -> RS:1.000.000 ", wallet->amount);
	return;
};
void LogHours(float hours, float minutes, int x, int y, ALLEGRO_FONT *font, ALLEGRO_COLOR cor) {
	al_draw_textf(font, cor, x, y, ALLEGRO_ALIGN_CENTRE, "%d : %d0", lround(hours * 4.8), lround(minutes * 28));
	return;
}
void ShowMoney(int x, int y, int value, ALLEGRO_FONT* font) 
{
	al_draw_textf(font, al_map_rgb(255, 255, 255), x, y, ALLEGRO_ALIGN_CENTRE, "RS:%d", value);
	return;
}
void LogQtyInvestCompany(int x, int y, int value, ALLEGRO_FONT* font) 
{
	al_draw_textf(font, al_map_rgb(255, 255, 255), x, y, ALLEGRO_ALIGN_CENTRE, "%i", value);
	return;
}


void InitWallet(Wallet* wallet) {
	wallet->amount = 100;
	wallet->investedProfit = 0;
	wallet->lossPerDay = 0;
	wallet->profitPerDay = 0;
	wallet->products = 1;
	wallet->branches = 1;
	wallet->employers = 1;
	wallet->valueCompany = 1;
	wallet->InsecureInvestedAmount = 1;
	wallet->chanceToWin = 65;
}
void AddAmount(Wallet* wallet) {
	wallet->amount+=50;
}
void InvestmentReturn(Wallet* wallet)
{
	wallet->amount += ((wallet->safeInvestedAmount) * 2 / 100) + wallet->valueCompany* 2;
	wallet->safeInvestedAmount += (wallet->safeInvestedAmount) * 2 / 100;
}
void RemoveBalance(Wallet* wallet, Container* container)
{
	wallet->amount -= wallet->valueCompany *5;

	int reponse = 0;

	if (wallet->amount < 0)
		reponse = al_show_native_message_box(NULL, "Ops!", "voce perdeu:", "Pressione F\nA sua empresa abriu falencia \nDeseja reiniciar o Jogo", NULL, ALLEGRO_MESSAGEBOX_YES_NO);

	if (reponse == 2)
		container->hasFinished = true;

}


void InitTimerGame(TimerGame *timerGame)
{
	timerGame->hours = 4.5826;
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
	al_draw_filled_rectangle(mouse->x, mouse->y, mouse->x + 10, mouse->y + 10, al_map_rgb(44, 117, 255));

	//al_draw_bitmap(mouse->image, mouse->x, mouse->y, 0);
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
	InitBackground(background, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_load_bitmap("src/imgs/oficce.bmp"), al_load_bitmap("src/imgs/pcMode.png"));
	
	// Criando Personagem
	InitPlayer(player);

	// Carrega tempo de jogo
	InitTimerGame(timerGame);

	// Criando Mouse
	initMouse(mouse);

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
		printf("%d : %d \n", lround(timerGame->hours * 4.8), lround(timerGame->minutes * 28));
		al_draw_bitmap(background->image, background->x, background->y + 120, 0);
		
		if (lround(timerGame->hours * 4.8) < 19 && lround(timerGame->hours * 4.8) > 6) {
			al_draw_filled_rectangle(0, 120, WINDOW_WIDTH, 0, al_map_rgb(65, 166, 246));

			InitMovingCloudBackground(&background->cloud);
			DrawCloudBackground(&background->cloud);
		}
		else if (lround(timerGame->hours * 4.8) == 19 && lround(timerGame->minutes * 28) == 0)
		{
			fadeInNight(background, 1, container, timerGame);
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


void fadeInNight(Background* background, float speed, Container *container, TimerGame* timer) {


	ALLEGRO_BITMAP* image = NULL;
	int y;

	if (container->isPcMode)
	{
		image = background->pcBackground.image;
		y = background->y;
	}
	else
	{
		image = background->image;
		y = background->y + 120;
	}

	timer->minutes += 0.034716;

	int alfa;
	for (alfa = 0; alfa <= 255; alfa += 25)
	{

		al_flip_display();

		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_tinted_bitmap(image, al_map_rgba(alfa, alfa, alfa, alfa), background->x, y, 0);

		al_rest(0.1);
		
	}
	return;
}


void statusProgress(Wallet* wallet, int x1, int y1, int x2, int y2) 
{
	int percentageMoney = wallet->amount * 100/ 1000000;
	int sizeRectangle = ((x2 - x1) * percentageMoney / 100) + x1 + 30;

	al_draw_filled_rectangle(x1, y1, sizeRectangle, y2, al_map_rgb(83, 255, 74));
};


void InitEvent(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Wallet* wallet)
{
	ALLEGRO_TIMER* timer = NULL;
	timer = al_create_timer(1.0 / FPS);

	container->hasFinished = false;
	container->needRedraw = false;
	container->isPcMode = false;

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

			// 1 para 1
			// seconds = 0,0005786
			// minutes = 0,034716
			// hours   = 0,2083

			timerGame->seconds += 0.0005786;
			if (timerGame->seconds >= 0.034716)
			{
				timerGame->seconds = 0;
				timerGame->minutes += 0.034716;

				if (timerGame->minutes >= 0.2083)
				{
					timerGame->hours += 0.2083;
					timerGame->minutes = 0;

					if (timerGame->hours >= 4.9992)
					{
						timerGame->hours = 0;
						InvestmentReturn(wallet);
						attTimerGame(timerGame, 1, 60, 3600, 1);
						RemoveBalance(wallet, container);
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
			case ALLEGRO_KEY_ENTER:

				container->needRedraw = true;
				container->isPcMode = !container->isPcMode;

				fadeInNight(background, 1, container, timerGame, player);

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

				// botão de add valor investimento seguro
				if (mouse->x >= 335 && mouse->x <= 490 && mouse->y >= 325 && mouse->y <= 370 && wallet->amount > investSafeForValue * 2)
					investSafeForValue *= 2;

				// botão de investir seguro
				if (mouse->x >= 360 && mouse->x <= 460 && mouse->y >= 470 && mouse->y <= 500 && wallet->amount > investSafeForValue)
				{
					wallet->amount -= investSafeForValue;
					wallet->safeInvestedAmount += investSafeForValue;

					investSafeForValue = 5;
				}

				// Add investimento na empresa: Product
				if (mouse->x >= 224 && mouse->x <= 296 && mouse->y >= 440 && mouse->y <= 460 && wallet->amount > wallet->valueCompany)
				{
					wallet->amount -= wallet->valueCompany;
					wallet->valueCompany *= 3;
					wallet->products++;
				}

				// Add investimento na empresa: employers
				if (mouse->x >= 224 && mouse->x <= 296 && mouse->y >= 325 && mouse->y <= 345 && wallet->amount > wallet->valueCompany)
				{
					wallet->amount -= wallet->valueCompany;
					wallet->valueCompany *= 2;
					wallet->employers++;
				}

				// Add investimento na empresa: branches
				if (mouse->x >= 224 && mouse->x <= 296 && mouse->y >= 385 && mouse->y <= 405 && wallet->amount > wallet->valueCompany)
				{
					wallet->amount -= wallet->valueCompany;
					wallet->valueCompany *= 4;
					wallet->branches++;
				}

				// botão de add valor investimento inseguro
				if (mouse->x >= 535 && mouse->x <= 692 && mouse->y >= 320 && mouse->y <= 340 && wallet->amount > wallet->InsecureInvestedAmount * 2)
				{
					wallet->InsecureInvestedAmount *= 2;
				}

				// botão de investir inseguro
				if (mouse->x >= 572 && mouse->x <= 650 && mouse->y >= 474 && mouse->y <= 500 && wallet->amount > wallet->InsecureInvestedAmount)
				{

					wallet->chanceToWin = 100;

					int diferenceToCalc = (wallet->chanceToWin - 100) * (-1);
					int addToChance = wallet->chanceToWin - diferenceToCalc;
					int chance = rand() % 99 + 1;

					if ((chance + addToChance) > 50)
					{
						wallet->amount += 100000;
					}
					else
					{
						wallet->amount -= 50;
					}
				}


			}
			if (event.mouse.button & 2) { // botão direito

			}
		}

		if (player->needRedraw)
			player->sourceX += al_get_bitmap_width(player->spritePlayer) / 4;
		else
			player->sourceX = 0;


		if (player->sourceX >= al_get_bitmap_width(player->spritePlayer))
			player->sourceX = 0;

		player->sourceY = dir * al_get_bitmap_height(player->spritePlayer) / 4;

		if (container->needRedraw && !container->isPcMode)
		{
			container->needRedraw = false;

			DrawPlayer(player);

			LogHours(timerGame->hours, timerGame->minutes, 500, 140, fontTimer, al_map_rgb(255, 0, 0));

			al_flip_display();

			DrawBackground(background, timerGame, container);

			DrawCloudBackground(background->cloud);
			InitMovingCloudBackground(background->cloud);
			attCloudPosition(background->cloud);

		}
		else if (container->needRedraw && container->isPcMode)
		{

			container->needRedraw = false;

			LogHours(timerGame->hours, timerGame->minutes, 960, 625, fontTimer, al_map_rgb(0, 0, 0));

			// Exibi os valores de dinheiro
			statusProgress(wallet, 107, 248, 385, 265);
			LogWallet(fontInvest, wallet);

			// valor investido
			ShowMoney(415, 358, investSafeForValue, fontInvest);

			// valor para investir
			ShowMoney(415, 445, wallet->safeInvestedAmount, fontInvest);

			// Products
			LogQtyInvestCompany(195, 449, wallet->products, fontInvest);

			// Branches
			LogQtyInvestCompany(195, 393, wallet->branches, fontInvest);

			// Employers
			LogQtyInvestCompany(195, 336, wallet->employers, fontInvest);

			// Valor do upgrade empresa:
			al_draw_textf(fontInvest, al_map_rgb(0, 0, 0), 195, 300, ALLEGRO_ALIGN_CENTRE, "Valor de upgrade RS:%i", wallet->valueCompany);

			// Valor do upgrade empresa:
			al_draw_textf(fontInvest, al_map_rgb(0, 0, 0), 580, 239, ALLEGRO_ALIGN_CENTRE, "Sua empresa ganha RS:%i/dia", wallet->valueCompany * 2);
			al_draw_textf(fontInvest, al_map_rgb(0, 0, 0), 580, 264, ALLEGRO_ALIGN_CENTRE, "Sua empresa gasta RS:%i/dia", wallet->valueCompany * 5);

			// Valor investido
			al_draw_textf(fontInvest, al_map_rgb(255, 255, 255), 650, 335, ALLEGRO_ALIGN_CENTRE, "%i", wallet->InsecureInvestedAmount);

			// Valor chance de ganhar
			al_draw_textf(fontInvest, al_map_rgb(255, 255, 255), 550, 379, ALLEGRO_ALIGN_CENTRE, "%i", wallet->chanceToWin);

			// Valor ganho
			al_draw_textf(fontInvest, al_map_rgb(255, 255, 255), 580, 445, ALLEGRO_ALIGN_CENTRE, "%i", wallet->valueToWin);

			DrawMouse(mouse);

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