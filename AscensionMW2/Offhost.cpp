#include <string.h>

#include "Offhost.h"
#include "Functions.h"
#include "PS3.h"

float MENU_X = -1000;
float MENU_Y = 100;
float MENU_WIDTH = 200;
float MENU_HEIGHT = 300;

void ChangeMenu() {
	int currentMenu = Ascension.menu;
	int currentMenuPosition = Ascension.position;
	switch (currentMenu) {
	case 0:
		switch (currentMenuPosition) {
		case 0:
			Ascension.menu = 1;
			break;
		case 1:
			Ascension.menu = 2;
			break;
		case 2:
			Ascension.menu = 3;
			break;
		case 3:
			Ascension.menu = 4;
			break;
		case 4:
			Ascension.menu = 5;
			break;
		case 5:
			Ascension.menu = 6;
			break;
		}
		break;
		case 6:
			if (IsInGame() && cg != NULL) {
				memcpy(&Ascension.currentClient, &Ascension.position, sizeof(int));
				const char *clientName = cg->info[Ascension.currentClient].name;
				if (strlen(clientName))
					Ascension.menu = 7;
			}
			break;
	}

	Ascension.Scroll(0);
}

int32_t Offhost::StartMenu() {
	int32_t wp_result = WriteProcessMemory<bool>((uint32_t)&this->init, true, sizeof(bool));
	return wp_result;
}

void Offhost::Open() {
	this->isOpen = true;
	this->MenuTransitionIn(150);
}

void Offhost::MenuTransitionIn(usecond_t ms) {
	this->CalculateMenuHeight();
	float step = (1000 / ms);

	for (int i = 0; i < ms; i++) {
		MENU_X += step;
		Sleep(1);
	}

	MENU_X = 0;
}

void Offhost::Close() {
	this->MenuTransitionOut(150);
	this->isOpen = false;
}

void Offhost::MenuTransitionOut(usecond_t ms) {
	this->CalculateMenuHeight();
	float step = (1000 / ms);

	for (int i = 0; i < ms; i++) {
		MENU_X += step;
		Sleep(1);
	}

	MENU_X = -1000;
}

void Offhost::CalculateMenuHeight() {
	MENU_HEIGHT = this->menu != 6 ? 35 + (this->GetCurrentMenu().count * 14) : 35 + (GetMaxClients() * 14);
}

void Offhost::LoadMenu(usecond_t ms) {
	float currentPosition = this->scrollbarPosition;
	float targetPosition = (MENU_Y + 28) + (this->position * 14);
	float NEW_HEIGHT = this->menu != 6 ? 35 + (this->GetCurrentMenu().count * 14) : 35 + (GetMaxClients() * 14);
	
	float stepH = (NEW_HEIGHT - MENU_HEIGHT) / ms;
	float stepS = (targetPosition - currentPosition) / ms;

	for (int i = 0; i < ms; i++) {
		MENU_HEIGHT += stepH;
		this->scrollbarPosition += stepS;
		Sleep(1);
	}
	
	MENU_HEIGHT = NEW_HEIGHT;
	this->scrollbarPosition = targetPosition;
}

void Offhost::UpdateMenuHeight(usecond_t ms) {
	float NEW_HEIGHT = this->menu != 6 ? 35 + (this->GetCurrentMenu().count * 14) : 35 + (GetMaxClients() * 14);

	if (MENU_HEIGHT > NEW_HEIGHT) {
		float step = (MENU_HEIGHT - NEW_HEIGHT) / ms;
		for (float i = MENU_HEIGHT; i > NEW_HEIGHT; i -= step) {
			MENU_HEIGHT = i;
			Sleep(1);
		}
	} else {
		float step = (NEW_HEIGHT - MENU_HEIGHT) / ms;
		for (float i = MENU_HEIGHT; i < NEW_HEIGHT; i += step) {
			MENU_HEIGHT = i;
			Sleep(1);
		}
	}

	MENU_HEIGHT = NEW_HEIGHT;
}

void Offhost::MoveScrollbar(usecond_t ms) {
	float currentPosition = this->scrollbarPosition;
	float targetPosition = (MENU_Y + 28) + (this->position * 14);
	
	if (currentPosition > targetPosition) {
		float step = (currentPosition - targetPosition) / ms;
		for (float pos = currentPosition; pos > targetPosition; pos -= step) {
			this->scrollbarPosition = pos;
			Sleep(1);
		}
	} else if (currentPosition < targetPosition) {
		float step = (targetPosition - currentPosition) / ms;
		for (float pos = currentPosition; pos < targetPosition; pos += step) {
			this->scrollbarPosition = pos;
			Sleep(1);
		}
	}

	this->scrollbarPosition = targetPosition;
}

int Offhost::Scroll(int n) {
	int oldPos = this->position = this->menus[this->menu].position;
	int newPos = oldPos + n;
	int optionCount = this->menus[this->menu].count;

	if (newPos > optionCount - 1)
		newPos = 0;
	else if (newPos < 0)
		newPos = optionCount - 1;

	this->position = newPos;
	this->menus[this->menu].position = newPos;

	if (n == 0) 
		this->LoadMenu(100);
	else
		this->MoveScrollbar(100);

	return newPos;
}

int Offhost::Increment(int n) {
	int oldPos, newPos;
	SubMenuOpt currentOption = this->GetCurrentOption();
	if (currentOption.scale.value != NULL) {
		oldPos = *currentOption.scale.value;
		newPos = oldPos + (currentOption.scale.interval * n);

		if (newPos > currentOption.scale.maxValue)
			newPos = currentOption.scale.minValue;
		else if (newPos < currentOption.scale.minValue)
			newPos = currentOption.scale.maxValue;
		
		if (!this->Variables.rainbowTheme)
			*currentOption.scale.value = newPos;

	} else if (currentOption.slider.value != NULL) {
		oldPos = *currentOption.slider.value;
		newPos = oldPos + n;

		if (newPos > currentOption.slider.maxValue)
			newPos = currentOption.slider.minValue;
		else if (newPos < currentOption.slider.minValue)
			newPos = currentOption.slider.maxValue;
		
		*currentOption.slider.value = newPos;
	}

	return newPos;
}

void Offhost::Select() {
	SubMenuOpt option = this->GetCurrentOption();
	if (option.boolean != NULL) *option.boolean = !*option.boolean;
	option.function();
}

void Offhost::GoBack() {
	int menu = this->menu;
	this->menu = this->menus[menu].parent;
	this->Scroll(0);
	this->UpdateMenuHeight(100);
}

void Offhost::BuildMenu() {
	this->init = true;
	this->isOpen = false;
	this->color[0] = 1.f;
	this->color[1] = 0.f;
	this->color[2] = 0.f;
	this->color[3] = 1.f;
	this->themeColor[0] = 255.f;
	this->themeColor[1] = 0.f;
	this->themeColor[2] = 0.f;
	this->themeColor[3] = 255.f;
	this->menu = 0;
	this->position = 0;
	this->scrollbarPosition = MENU_Y + 28;

	this->Variables.antiCFG = false;
	this->Variables.rainbowTheme = true;
	this->Variables.coloredMenus = false;
	this->Variables.ghostBG = true;
	this->Variables.flashName = false;
	this->Variables.showControls = true;

	this->Variables.sweepingUAV = false;
	this->Variables.depatchBounces = false;
	this->Variables.depatchElevators = false;
	this->Variables.minisculeHealth = false;
	this->Variables.noFallingDamage = false;

	this->Variables.minPlayers = 4;
	this->Variables.maxPlayers = 12;

	this->AddMenu(-1, 0, "Main Menu");
	this->AddMenu(0, 1, "Names Menu");
		this->AddOption(1, "Change Name", &ChangeName);
		this->AddBool(1, "Flash Name", &ToggleFlashName, &this->Variables.flashName);
		this->AddOption(1, "Reset Name", &ResetName);
	this->AddMenu(0, 2, "Recovery Menu");
		this->AddOption(2, "Level 70", &SetLevel70);
		this->AddSlider(2, "Prestige", &SetPrestige, &this->Variables.prestige, 0, 11);
		this->AddOption(2, "Unlock All", &UnlockAll);
		this->AddOption(2, "Save Stats", &SaveStats);
		this->AddOption(2, "Load Stats", &LoadStats);
	//	this->AddBool(2, "Auto Load Stats", &Test, &this->Variables.autoLoad);
	this->AddMenu(0, 3, "Game Settings");
		this->AddBool(3, "Anti CFG", &Test, &this->Variables.antiCFG);
		this->AddBool(3, "Prevent Enemy Forfeit", &PreventForfeit, &this->Variables.preventEnemyForfeit);
		this->AddBool(3, "Sweeping UAV", &ConstantUAV, &this->Variables.sweepingUAV);
		this->AddBool(3, "Depatch Bounces", &ToggleBounces, &this->Variables.depatchBounces);
		this->AddBool(3, "Depatch Elevators", &ToggleElevators, &this->Variables.depatchElevators);
		this->AddBool(3, "Miniscule Health", &Test, &this->Variables.minisculeHealth);
		this->AddBool(3, "Remove Falling Damage", &Test, &this->Variables.noFallingDamage);
	this->AddMenu(0, 4, "Matchmaking Menu");
		this->AddBool(4, "Force Host", &ToggleForceHost, &this->Variables.forceHost);
		this->AddSlider(4, "Players to Start", &Test, &this->Variables.minPlayers, 1, 18);
		this->AddSlider(4, "Max Players", &UpdateLobbySize, &this->Variables.maxPlayers, 1, 18);
		this->AddBool(4, "Disable DLC", &ToggleDLC, &this->Variables.disableDLC);
		this->AddOption(4, "Change Map", &OpenMapSelectMenu);
		this->AddOption(4, "Change Game Mode", &OpenGamemodeSelectMenu);
	this->AddMenu(0, 5, "Menu Settings");
		this->AddBool(5, "Show Controls", &Test, &this->Variables.showControls);
		this->AddBool(5, "Colored Menus", &Test, &this->Variables.coloredMenus);
		this->AddBool(5, "Ghost Background", &Test, &this->Variables.ghostBG);
		this->AddBool(5, "Rainbow Theme", &Test, &this->Variables.rainbowTheme);
		this->AddScale(5, "Theme: Red", &Test, &this->themeColor[0], 0, 255, 1, COLOR_RED);
		this->AddScale(5, "Theme: Green", &Test, &this->themeColor[1], 0, 255, 1, COLOR_GREEN);
		this->AddScale(5, "Theme: Blue", &Test, &this->themeColor[2], 0, 255, 1, COLOR_BLUE);
	this->AddMenu(0, 6, "Player List");
		for (int i = 0; i < 18; i++)
			AddMenu(6, 7, "");
		
		AddOption(7, "Kick", &KickClient);
		AddOption(7, "Unlock All", &UnlockAllClient);

	this->AddOption(0, "Fast Restart", &FastRestart);
	this->AddOption(0, "Drop Canswap", &DropCanswap);
	this->AddOption(0, "Move Bot to Me", &MoveBot);
	
	Globals.spawnBots = false;
	Globals.dropCanswap = false;

	strcpy(xenonUserData->gamertag, this->Variables.gamertag);
}

void Offhost::DrawInfoText() {
	if (this->isOpen) {
		this->infoText = "Ascension: \x0014 \x0015 \x0016 \x0017 to Scroll | \x0003 to Select | \x0002 to Close";
	} else if (this->Variables.showControls) {
		this->infoText = "Ascension: Press \x0010 + \x0015 to Open";
		float width = GetTextWidth(this->infoText, FONT_NORMAL, 0.25);
		DrawShader(-102.5, 462.5, width + 5, 15, COLOR_BLACK_FADE, "white");
		DrawText(this->infoText, FONT_NORMAL, 0.25, -100, 475, COLOR_WHITE);
	}
}

void Offhost::DrawMenuShaders() {
	float value, minValue, maxValue, interval;
	float steps, scaler, xOffset;

	float size = 65.f;

	DrawShader(-125, 0, 1920, 1080, MENU_OVERLAY, "white");
	DrawShader(MENU_X + CalculateCenterWidthAdjustment(MENU_WIDTH), MENU_Y, MENU_WIDTH, MENU_HEIGHT, COLOR_BLACK_FADE, "white");
	DrawShader(MENU_X + CalculateCenterWidthAdjustment(MENU_WIDTH), MENU_Y, 1.5, MENU_HEIGHT, this->color, "white");
	DrawShader(MENU_X + CalculateCenterWidthAdjustment(MENU_WIDTH) + MENU_WIDTH, MENU_Y, 1.5, MENU_HEIGHT, this->color, "white");
	DrawShader(MENU_X + CalculateCenterWidthAdjustment(MENU_WIDTH), this->scrollbarPosition, MENU_WIDTH, 13, this->color, "white");

	SubMenu currentMenu = this->GetCurrentMenu();
	for (int i = 0; i < currentMenu.count; i++) {
		if (currentMenu.options[i].scale.color != NULL) {
			DrawShader((MENU_X + CalculateCenterWidthAdjustment(MENU_WIDTH) + MENU_WIDTH) - (size + 5), 133.5 + (i * 14), size, 0.75, COLOR_WHITE_FADE, "white");
			
			value = static_cast<float>(*currentMenu.options[i].scale.value);
			minValue = static_cast<float>(currentMenu.options[i].scale.minValue);
			maxValue = static_cast<float>(currentMenu.options[i].scale.maxValue);
			interval = static_cast<float>(currentMenu.options[i].scale.interval);

			steps = ((maxValue - minValue) / interval);
			scaler = (size / (steps / (value / maxValue)));
			xOffset = (scaler * steps);

			DrawShader(((MENU_X + CalculateCenterWidthAdjustment(MENU_WIDTH) + MENU_WIDTH) - (size + 5)) + xOffset, 130 + (i * 14), 2, 7, currentMenu.options[i].scale.color, "white");
		}
	}
}

void Offhost::DrawMenuText() {
	const char *menuTitle = "Ascension";
	float width1 = GetTextWidth(menuTitle, FONT_NORMAL, 0.35);
	DrawTextWithGlow(menuTitle, FONT_NORMAL, 0.35, MENU_X + CalculateCenterWidthAdjustment(width1), MENU_Y + 20, COLOR_WHITE, this->color);

	SubMenu currentMenu = this->GetCurrentMenu();
	for (int i = 0; i < currentMenu.count; i++) {
		const char *opt = currentMenu.options[i].title;
		DrawText(opt, FONT_NORMAL, 0.25, MENU_X +  CalculateCenterWidthAdjustment(MENU_WIDTH) + 5, (MENU_Y + 40) + (i * 14), COLOR_WHITE);

		if (currentMenu.options[i].boolean != NULL) {
			const char *text = "OFF";
			if (*currentMenu.options[i].boolean) 
				text = "ON";

			float width2 = GetTextWidth(text, FONT_NORMAL, 0.25);
			DrawText(text, FONT_NORMAL, 0.25, MENU_X + CalculateCenterWidthAdjustment(MENU_WIDTH) + (MENU_WIDTH - width2 - 5), 140 + (i * 14), COLOR_WHITE);
		} else if (currentMenu.options[i].slider.value != NULL) {
			const char *text = va("%i", *currentMenu.options[i].slider.value);
			float width2 = GetTextWidth(text, FONT_NORMAL, 0.25);
			DrawText(text, FONT_NORMAL, 0.25, MENU_X + CalculateCenterWidthAdjustment(MENU_WIDTH) + (MENU_WIDTH - width2 - 5), 140 + (i * 14), COLOR_WHITE);
		}
	}

	DrawShader(-102.5, 462.5, GetTextWidth(this->infoText, FONT_NORMAL, 0.25) + 5, 15, COLOR_BLACK_FADE, "white");
	DrawText(this->infoText, FONT_NORMAL, 0.25, -100, 475, COLOR_WHITE);
}

void Offhost::DrawPlayersMenu() {
	const char *menuTitle = "Ascension";
	float width1 = GetTextWidth(menuTitle, FONT_NORMAL, 0.35);
	DrawTextWithGlow(menuTitle, FONT_NORMAL, 0.35, MENU_X + CalculateCenterWidthAdjustment(width1), MENU_Y + 20, COLOR_WHITE, this->color);

	SubMenu currentMenu = this->GetCurrentMenu();
	char clientName[MAX_CLIENTS][MAX_NETNAME];
	for (int i = 0; i < GetMaxClients(); i++) {
		ClientCleanName(SV_GetClientName(i), clientName[i], MAX_NETNAME);
		float w = GetTextWidth(clientName[i], FONT_NORMAL, 0.25);
		DrawText(clientName[i], FONT_NORMAL, 0.25, MENU_X +  CalculateCenterWidthAdjustment(MENU_WIDTH) + 5, (MENU_Y + 40) + (i * 14), COLOR_WHITE);
	}
}

void Offhost::AddOption(int menu, const char *title, MenuFunction *function) {
	char index = this->menus[menu].count;
	this->menus[menu].options[index].title = title;
	this->menus[menu].options[index].boolean = NULL;
	this->menus[menu].options[index].slider.value = NULL;
	this->menus[menu].options[index].scale.value = NULL;
	this->menus[menu].options[index].function = function;
	this->menus[menu].count++;
}

void Offhost::AddSlider(int menu, const char *title, MenuFunction *function, int *value, int minValue, int maxValue) {
	char index = this->menus[menu].count;
	this->menus[menu].options[index].title = title;
	this->menus[menu].options[index].boolean = NULL;
	this->menus[menu].options[index].slider.value = value;
	this->menus[menu].options[index].slider.minValue = minValue;
	this->menus[menu].options[index].slider.maxValue = maxValue;
	this->menus[menu].options[index].function = function;
	this->menus[menu].count++;
}

void Offhost::AddBool(int menu, const char *title, MenuFunction *function, bool *boolean) {
	char index = this->menus[menu].count;
	this->menus[menu].options[index].title = title;
	this->menus[menu].options[index].boolean = boolean;
	this->menus[menu].options[index].function = function;
	this->menus[menu].count++;
}

void Offhost::AddMenu(int parent, int menu, const char *title) {
	this->menus[menu].parent = parent;
	this->menus[menu].title = title;
	this->menus[menu].position = 0;
	if (!this->menus[menu].count) this->menus[menu].count = -1;
	this->menus[menu].count++;

	if (parent != -1) this->AddOption(parent, title, &ChangeMenu);
}

void Offhost::AddScale(int menu, const char *title, MenuFunction *function, float *value, float minValue, float maxValue, float interval, const float *color) {
	char index = this->menus[menu].count;
	this->menus[menu].options[index].title = title;
	this->menus[menu].options[index].boolean = NULL;
	this->menus[menu].options[index].scale.value = value;
	this->menus[menu].options[index].scale.minValue = minValue;
	this->menus[menu].options[index].scale.maxValue = maxValue;
	this->menus[menu].options[index].scale.interval = interval;
	this->menus[menu].options[index].function = function;
	this->menus[menu].options[index].scale.color = color;
	this->menus[menu].count++;
}

SubMenu Offhost::GetCurrentMenu() {
	return this->menus[this->menu];
}

SubMenuOpt Offhost::GetCurrentOption() {
	return this->menus[this->menu].options[this->position];
}

Offhost Ascension;

globals_s Globals;