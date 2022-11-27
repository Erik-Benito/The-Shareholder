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


// Prototypes
void LogError(char* error);
void LogFrames(int fps, ALLEGRO_FONT* font, ALLEGRO_COLOR* COR);
void LogHours(float hours, float minutes, int x, int y, ALLEGRO_FONT* font, ALLEGRO_COLOR cor);
void ShowMoney(int x, int y, int value, ALLEGRO_FONT* font);
void LogQtyInvestCompany(int x, int y, int value, ALLEGRO_FONT* font);
void LogWallet(ALLEGRO_FONT* font, Wallet* wallet);

void InvestmentReturn(Wallet* wallet);
void AddAmount(Wallet* wallet);
void RemoveBalance(Wallet* wallet, Container* container);

void InitContainer(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Wallet* wallet, Hitbox hitboxs[5]);
void EndContainer(Container* container, Player* player);

void InitWallet(Wallet* wallet);
void statusProgress(Wallet* wallet, int x1, int y1, int x2, int y2);

void InitBackground(Background* background, float x, float y, int width, int height, ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* imagePc);
void DrawBackground(Background* background, TimerGame* timerGame, Container* container, Wallet* wallet);

void InitMovingCloudBackground(Cloud cloud[]);
void attCloudPosition(Cloud cloud[]);
void DrawCloudBackground(Cloud cloud[]);
void fadeInNight(Background* background, float speed, Container* container, TimerGame* time);

void InitTimerGame(TimerGame* timerGame);
void attTimerGame(TimerGame* timerGame, int hours, int minutes, int seconds, int days);

void InitPlayer(Player* player);
void DrawPlayer(Player* player);

void InitEvent(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Wallet* wallet, Hitbox hitboxs[5]);
void ControlEvent(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Wallet* wallet, Hitbox hitboxs[5]);

void initMouse(Mouse* mouse);
void DrawMouse(Mouse* mouse);

void PlayerMoveUp(Player* player);
void PlayerMoveDown(Player* player);
void PlayerMoveRight(Player* player);
void PlayerMoveLeft(Player* player);

void InitCollision(Player* player, Hitbox hitboxs[5]);
bool HasCollision(Player* player, Hitbox hitboxs[5]);

//=================================//

Player player;
Container container;
Background background;
TimerGame timerGame;
Hitbox hitboxs[5];
Mouse mouse;
Wallet wallet;

int main(void) {

	InitContainer(&container, &background, &player, &mouse, &timerGame, &wallet, &hitboxs);
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
	al_draw_textf(font, al_map_rgb(0, 0, 0), 152, 252, ALLEGRO_ALIGN_CENTER, "RS:%d", wallet->amount);
	return;
};
void LogHours(float hours, float minutes, int x, int y, ALLEGRO_FONT* font, ALLEGRO_COLOR cor) {
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
void LogProfitOrLoss(Wallet* wallet, ALLEGRO_FONT* font)
{
	if(wallet->hasWin)
		al_draw_textf(font, al_map_rgb(0, 255, 0), 610, 293, ALLEGRO_ALIGN_CENTRE, "+%i",  wallet->valueToWin * 2);
	else
		al_draw_textf(font, al_map_rgb(255, 0, 0), 610, 293, ALLEGRO_ALIGN_CENTRE, "-%i", wallet->valueToWin * 2);

	return;
}

// (log(10,20))^(x)

void InitWallet(Wallet* wallet) {
	wallet->amount = 100;
	wallet->investedProfit = 0;
	wallet->lossPerDay = 5;
	wallet->profitPerDay = 0;
	wallet->products = 1;
	wallet->branches = 1;
	wallet->employers = 1;
	wallet->valueCompany = 1;
	wallet->InsecureInvestedAmount = 2;
	wallet->chanceToWin = 50;
	wallet->valueToWin = 5000;
	wallet->safeInvestedAmount = 0;

}
void AddAmount(Wallet* wallet) {
	wallet->amount += 50;
}
void InvestmentReturn(Wallet* wallet)
{

	wallet->safeInvestedAmount += (wallet->safeInvestedAmount) * 2 / 100;
	wallet->amount += wallet->safeInvestedAmount + log10(wallet->valueCompany) * 0.8;

	wallet->lossPerDay += wallet->valueCompany;
	wallet->profitPerDay = 0.8 * (wallet->valueCompany) * log10(wallet->valueCompany);
}
void RemoveBalance(Wallet* wallet, Container* container)
{
	wallet->amount -= wallet->lossPerDay;
}


void InitTimerGame(TimerGame* timerGame)
{
	timerGame->hours = 4.5826;
	timerGame->minutes = 0;
	timerGame->seconds = 0;

	timerGame->hoursTotal = 0;
	timerGame->minutesTotal = 0;
	timerGame->secondsTotal = 0;
	timerGame->days = 0;

}
void attTimerGame(TimerGame* timerGame, int hours, int minutes, int seconds, int days)
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
	player->fpsPlayer = 0;


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


void InitCollision(Player* player, Hitbox hitboxs[5]) {

	// Player
	hitboxs[5].initX = 10;
	hitboxs[5].endX = 80;
	hitboxs[5].initY = 110;
	hitboxs[5].endY = 140;

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


bool HasCollision(Player* player, Hitbox hitboxs[5]) {

	// Plaver
	hitboxs[5].initX += 10;
	hitboxs[5].endX += 80;
	hitboxs[5].initY += 110;
	hitboxs[5].endY += 140;

	bool HasCollisionResponse = false;

	if (hitboxs[5].endX > hitboxs[0].initX && hitboxs[5].initX < hitboxs[0].endX && hitboxs[5].endY > hitboxs[0].initY && hitboxs[5].initY < hitboxs[0].endY)
		HasCollisionResponse = true;
	else if (hitboxs[5].endX > hitboxs[1].initX && hitboxs[5].initX < hitboxs[1].endX && hitboxs[5].endY > hitboxs[1].initY && hitboxs[5].initY < hitboxs[1].endY)
		HasCollisionResponse = true;
	else if (hitboxs[5].endX > hitboxs[2].initX && hitboxs[5].initX < hitboxs[2].endX && hitboxs[5].endY > hitboxs[2].initY && hitboxs[5].initY < hitboxs[2].endY)
		HasCollisionResponse = true;
	else if (hitboxs[5].initX < hitboxs[3].initX || hitboxs[5].endX  > hitboxs[3].endX)
		HasCollisionResponse = true;
	else if (hitboxs[5].initY < hitboxs[3].initY || hitboxs[5].endY  > hitboxs[3].endY)
		HasCollisionResponse = true;

	return HasCollisionResponse;
}


void InitContainer(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Wallet* wallet, Hitbox hitboxs[5])
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
	InitBackground(background, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_load_bitmap("src/imgs/oficce.bmp"), al_load_bitmap("src/imgs/pcMode.png"), al_load_bitmap("src/imgs/Office2.png"));

	// Criando Personagem
	InitPlayer(player);

	// Carrega tempo de jogo
	InitTimerGame(timerGame);

	// Criando Mouse
	initMouse(mouse);

	// Carrega a carteira do Player
	InitWallet(wallet);

	// Carregando Eventos do game 
	InitEvent(container, background, player, mouse, timerGame, wallet, hitboxs);

};
void EndContainer(Container* container, Player* player) {

	al_destroy_display(container->window);
	al_destroy_event_queue(container->eventQueue);
}


void InitBackground(Background* background, float x, float y, int width, int height, ALLEGRO_BITMAP* image, ALLEGRO_BITMAP* imagePc, ALLEGRO_BITMAP* office2)
{
	background->image = NULL;
	background->pcBackground.image = NULL;
	background->office2background.image = NULL;

	background->x = x;
	background->y = y;
	background->image = image;
	background->width = width;
	background->height = height;

	background->pcBackground.x = x;
	background->pcBackground.y = y;
	background->pcBackground.width = width;
	background->pcBackground.height = height;
	background->pcBackground.image = imagePc;

	background->office2background.x = x;
	background->office2background.y = y;
	background->office2background.width = width;
	background->office2background.height = height;
	background->office2background.image = office2;

	for (int i = 0; i < 5; i++)
	{
		background->cloud[i].speed = 15;
		background->cloud[i].active = false;
	}

	if (!background->pcBackground.image)
		LogError("Falha ao Carregar a imagem do computador");

	if (!background->image)
		LogError("Falha ao carregar a imagem do escritorio");
}
void DrawBackground(Background* background, TimerGame* timerGame, Container* container, Wallet* wallet)
{

	if (!container->isPcMode)
	{
		if (wallet->branches >= 10) {
			al_draw_bitmap(background->office2background.image, background->x, background->y + 120, 0);
		}
		else {
			al_draw_bitmap(background->image, background->x, background->y + 120, 0);
		}

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
			;
		}
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


void fadeInNight(Background* background, float speed, Container* container, TimerGame* timer) {


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
	int percentageMoney = wallet->amount * 100 / 1000000;
	int sizeRectangle   = ((x2 - x1) * percentageMoney / 100) + x1;

	al_draw_filled_rectangle(x1, y1, percentageMoney >= 100 ? x2 : sizeRectangle, y2, al_map_rgb(83, 255, 74));
};


void InitEvent(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Wallet* wallet, Hitbox hitboxs[5])
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

	InitCollision(player, hitboxs);
	ControlEvent(container, background, player, mouse, timerGame, wallet, hitboxs);
}
void ControlEvent(Container* container, Background* background, Player* player, Mouse* mouse, TimerGame* timerGame, Wallet* wallet, Hitbox hitboxs[5])
{
	ALLEGRO_FONT* font = al_load_font("src/font/BAVEUSE.TTF", 20, NULL);
	ALLEGRO_FONT* fontInvest = al_load_font("src/font/InvestFont.ttf", 12, NULL);
	ALLEGRO_FONT* fontText = al_load_font("src/font/KOMTXTK_.ttf", 16, NULL);
	ALLEGRO_FONT* fontTimer = al_load_font("src/font/MINECRAFT.TTF", 20, NULL);
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

	int dir = DOWN, frame = 0, investSafeForValue = 5, investDangerForValue = 0;
	float actualTime = 0, anotherTime = 0.0034716;

	al_register_event_source(container->eventQueue, al_get_timer_event_source(timer));

	al_start_timer(timer);


	while (!container->hasFinished)
	{
		ALLEGRO_EVENT event;
		ALLEGRO_KEYBOARD_STATE keyState;
		player->fpsPlayer++;

		al_wait_for_event(container->eventQueue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			player->needRedraw = true;
			if (keys[UP])
			{
				hitboxs[5].initY = player->y - player->speed;
				hitboxs[5].endY = player->y - player->speed;
				hitboxs[5].initX = player->x;
				hitboxs[5].endX = player->x;

				if (!HasCollision(player, hitboxs))
					PlayerMoveUp(player);
			}
			else if (keys[DOWN]) {
				hitboxs[5].initY = player->y + player->speed;
				hitboxs[5].endY = player->y + player->speed;
				hitboxs[5].initX = player->x;
				hitboxs[5].endX = player->x;

				if (!HasCollision(player, hitboxs))
					PlayerMoveDown(player);
			}
			else if (keys[LEFT]) {
				hitboxs[5].initY = player->y;
				hitboxs[5].endY = player->y;
				hitboxs[5].initX = player->x - player->speed;
				hitboxs[5].endX = player->x - player->speed;

				if (!HasCollision(player, hitboxs))
					PlayerMoveLeft(player);
			}
			else if (keys[RIGHT]) {
				hitboxs[5].initY = player->y;
				hitboxs[5].endY = player->y;
				hitboxs[5].initX = player->x + player->speed;
				hitboxs[5].endX = player->x + player->speed;

				if (!HasCollision(player, hitboxs))
					PlayerMoveRight(player);
			}
			else
				player->needRedraw = false;

			// 1 para 1
			// seconds = 0,0005786
			// minutes = 0,0034716
			// hours   = 0,2083

			timerGame->seconds += 0.0005786;

			if (timerGame->seconds >= 0.034716)
			{
				timerGame->seconds = 0;
				timerGame->minutes += 0.0034716;
				anotherTime += 0.0034716;
				
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
			case ALLEGRO_KEY_E:
				if (player->x > 420 && player->x < 520 && player->y < 180)
				{
					if (timerGame->hours < 0.2083 * 6) {
						//                    |H |MIN |SEC  |Day
						float subAuxHours = (0.2083 * 6) - timerGame->hours;
						float subAuxMinutes = (0.0034716 * 59) - timerGame->minutes;
						float subAuxSeconds = (0.0005786 * 59) - timerGame->seconds;
						attTimerGame(timerGame, lround(subAuxHours * 4.8), lround(subAuxMinutes * 4.8), lround(subAuxSeconds * 4.8), 1);
						//----(Caso a hora seja MENOR que 6 AM)----//
						timerGame->hours = (0.2083 * 6);
					}
					else {
						float subAuxHours = (0.2083 * 23) - timerGame->hours;
						float subAuxMinutes = (0.0034716 * 59) - timerGame->minutes;
						float subAuxSeconds = (0.0005786 * 59) - timerGame->seconds;
						//                    |H |MIN |SEC  |Day
						attTimerGame(timerGame, lround(subAuxHours * 4.8), lround(subAuxMinutes * 4.8), lround(subAuxSeconds * 4.8), 1);
						//----(Caso a hora seja MAIOR que 6 AM)----//
						timerGame->hours = (0.2083 * 6);

					}

					InvestmentReturn(wallet);
					RemoveBalance(wallet, container);
				}

				break;
			}
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			mouse->x = event.mouse.x;
			mouse->y = event.mouse.y;

			printf("X:%d | Y:%d\n", mouse->x, mouse->y);
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (event.mouse.button & 1) {
				//botão esquerdo

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
					wallet->amount -= wallet->valueCompany * 2;
					wallet->valueCompany *= 2;
					wallet->products++;
				}

				// Add investimento na empresa: employers
				if (mouse->x >= 224 && mouse->x <= 296 && mouse->y >= 325 && mouse->y <= 345 && wallet->amount > wallet->valueCompany)
				{
					wallet->amount -= wallet->valueCompany * 2;
					wallet->valueCompany *=2;
					wallet->employers++;
				}

				// Add investimento na empresa: branches
				if (mouse->x >= 224 && mouse->x <= 296 && mouse->y >= 385 && mouse->y <= 405 && wallet->amount > wallet->valueCompany)
				{
					wallet->amount -= wallet->valueCompany * 2;
					wallet->valueCompany *= 2;
					wallet->branches++;
				}

				// botão de add valor investimento inseguro
				if (mouse->x >= 535 && mouse->x <= 692 && mouse->y >= 320 && mouse->y <= 340 && wallet->amount > wallet->InsecureInvestedAmount * 2 && wallet->InsecureInvestedAmount < 16)
				{
					wallet->valueToWin *= 2;
					wallet->InsecureInvestedAmount *= 2;
					wallet->chanceToWin -= 15;
				}
				
				
				if (mouse->x >= 535 && mouse->x <= 692 && mouse->y >= 420 && mouse->y <= 430 && wallet->amount >= wallet->valueToWin * 2)
				{
					wallet->valueToWin *= 2;
				}


				// botão de investir inseguro
				if (mouse->x >= 572 && mouse->x <= 650 && mouse->y >= 474 && mouse->y <= 500 && wallet->amount > wallet->valueToWin)
				{

					int chance = wallet->chanceToWin - 50;
					int sort = rand() % 99 + 1;


					if ((chance + sort) > 50)
					{
						wallet->amount += wallet->valueToWin * 2;
						LogProfitOrLoss(wallet, fontInvest);
						wallet->hasWin = true;
					}
					else
					{
						wallet->hasWin = false;
						wallet->amount -= wallet->valueToWin;
						LogProfitOrLoss(wallet, fontInvest);
					}
					anotherTime = timerGame->minutes;
					actualTime  = timerGame->minutes;
				}


			}
			if (event.mouse.button & 2) { // botão direito
				if (mouse->x >= 535 && mouse->x <= 692 && mouse->y >= 420 && mouse->y <= 430 && wallet->valueToWin > 100)
				{
					wallet->valueToWin /= 2;
				}
				if (mouse->x >= 535 && mouse->x <= 692 && mouse->y >= 320 && mouse->y <= 340)
				{
					if(wallet->InsecureInvestedAmount > 2)
					{
						wallet->InsecureInvestedAmount /= 2;
						wallet->valueToWin /= 2;
						wallet->chanceToWin += 15;
					}
				}
			}
		}

		if(player->fpsPlayer == 4)
		{
			if (player->needRedraw)
				player->sourceX += al_get_bitmap_width(player->spritePlayer) / 4;
			else
				player->sourceX = 0;


			if (player->sourceX >= al_get_bitmap_width(player->spritePlayer))
				player->sourceX = 0;


			player->fpsPlayer = 0;
		}

		player->sourceY = dir * al_get_bitmap_height(player->spritePlayer) / 4;

		if (container->needRedraw && !container->isPcMode)
		{
			container->needRedraw = false;

			DrawPlayer(player);

			LogHours(timerGame->hours, timerGame->minutes, 500, 140, fontTimer, al_map_rgb(255, 0, 0));

			al_flip_display();

			DrawBackground(background, timerGame, container, wallet);
			
			DrawCloudBackground(background->cloud);
			InitMovingCloudBackground(background->cloud);
			attCloudPosition(background->cloud);

		}
		else if (container->needRedraw && container->isPcMode)
		{

			container->needRedraw = false;

			LogHours(timerGame->hours, timerGame->minutes, 960, 625, fontTimer, al_map_rgb(0, 0, 0));

			// valor investido
			ShowMoney(411, 364, investSafeForValue, fontInvest);
			
			// valor para investir
			ShowMoney(410, 440, wallet->safeInvestedAmount, fontInvest);

			
			// Valor do upgrade empresa:
			al_draw_textf(fontInvest, al_map_rgb(0, 0, 0), 195, 300, ALLEGRO_ALIGN_CENTRE, "Valor de upgrade RS:%i", wallet->valueCompany * 2);

			// Products
			LogQtyInvestCompany(197, 454, wallet->products, fontInvest);

			// Branches
			LogQtyInvestCompany(197, 397, wallet->branches, fontInvest);

			// Employers
			LogQtyInvestCompany(197, 340, wallet->employers, fontInvest);


			// Valor chance de ganhar
			al_draw_textf(fontInvest, al_map_rgb(255, 255, 255), 555, 378, ALLEGRO_ALIGN_CENTRE, "%i", wallet->chanceToWin);

			// Valor investido inseguro
			al_draw_textf(fontInvest, al_map_rgb(255, 255, 255), 624, 448, ALLEGRO_ALIGN_CENTRE, "%i", wallet->valueToWin);

			// Valor mutiplicador
			al_draw_textf(fontInvest, al_map_rgb(255, 255, 255), 675, 334, ALLEGRO_ALIGN_CENTRE, "%i", wallet->InsecureInvestedAmount);


			// Carteira
			LogWallet(fontInvest, wallet);

			// Status de progess
			statusProgress(wallet, 340, 246, 485, 266);

			// TexT 1000000
			al_draw_textf(fontInvest, al_map_rgb(255, 255, 255), 405, 252, ALLEGRO_ALIGN_CENTRE, "RS:100.000.00");

			// Valor do upgrade empresa:
			al_draw_textf(fontInvest, al_map_rgb(0, 0, 0), 655, 227, ALLEGRO_ALIGN_CENTRE, "RS:%i", wallet->profitPerDay);
			al_draw_textf(fontInvest, al_map_rgb(0, 0, 0), 655, 254, ALLEGRO_ALIGN_CENTRE, "RS:%i",  wallet->lossPerDay);


			DrawMouse(mouse);


			if (mouse->x > 315 && mouse->x < 519)
			{
				al_draw_textf(fontText, al_map_rgb(0, 0, 0), 750, 154, NULL, "Investimento de Renda Fixa");
				al_draw_multiline_text(fontText, al_map_rgb(0, 0, 0), 737, 200, 820, 15, 0,"Os investimentos de Renda Fix\nmais procuradosao os\ntitulos do Tesouro Direto, CDBs,\ndebentures, Fundos de Renda Fixa\nLCI, LCAs, CRIS CRAs e as\ncarteiras digitais renuneradas\nA rentabilidade da Renda Fixa\ndepende do valor da Selic \nque, por sua vez,\ndeterminara o valor da taxa do CDI,\na principal medida de retorno\ndesse tipo de investinento.");
			}

			if (mouse->x < 300)
			{
				al_draw_textf(fontText, al_map_rgb(0, 0, 0), 750, 154, NULL, "Investimentir na empresa");
				al_draw_multiline_text(fontText, al_map_rgb(0, 0, 0), 737, 200, 820, 15, 0, "Ao investir na empresa,\nvoce tera melhoras no se ganho\nmas tambem aumentara\n os gastos com \nseus investimentos,\nentao e importante administrar a\nquantidade de investimento\npara que consiga\nconciliar seu ganho\ncom os gastos.\nAo aumentar o investimento \ne possivel ganhar melhorias\npara seu escritorio");
			}

			if (mouse->x > 516)
			{
				al_draw_textf(fontText, al_map_rgb(0, 0, 0), 750, 154, NULL, "Investimento de Renda Variavel");
				al_draw_multiline_text(fontText, al_map_rgb(0, 0, 0), 737, 200, 820, 15, 0, "Classe de investimentos\ncomposta por ativos e \nprodutos financeiros\nsobre os quais nao e possivel\n saber antecipadamente\ncomo(ou se) ocorrera a\nrentabilidade.\nPortanto, nao ha garantias ou \nprevisao a respeito do retorno\nque pode ser obtido.");
			}


			al_flip_display();
			DrawBackground(background, timerGame, container, wallet);

			if (anotherTime < actualTime + 3 * 0.0034716 && actualTime != 0)
			{
				LogProfitOrLoss(wallet, fontInvest);
			}
			else
			{
				anotherTime = 0;
				actualTime = 0;
			}
		}

		if (frame >= FPS)
			frame--;
		else
			frame++;


		int reponse = 0;

		if (wallet->amount < 0)
			reponse = al_show_native_message_box(NULL, "Ops!", "voce perdeu:", "Pressione F\nA sua empresa abriu falencia \nDeseja reiniciar o Jogo", NULL, ALLEGRO_MESSAGEBOX_YES_NO);

		if (reponse == 2)
			container->hasFinished = true;

		if (reponse == 1)
		{

			Player playerNew;
			Container containerNew;
			Background backgroundNew;
			TimerGame timerGameNew;
			Hitbox hitboxsNew[5];
			Mouse mouseNew;
			Wallet walletNew;

			EndContainer(container, player);
			InitContainer(&containerNew, &backgroundNew, &playerNew, &mouseNew, &timerGameNew, &walletNew, &hitboxsNew);

		}
	}

	EndContainer(container, player);
}