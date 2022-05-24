#ifndef _OFFHOST_
#define _OFFHOST_

#include <cell/pad/pad_codes.h>
#include <cell/pad/libpad.h>

#include "Struct.h"
#include "Drawing.h"

#define	SCREEN_WIDTH 640
#define	SCREEN_HEIGHT 480

#define MAX_MENU 10
#define MAX_OPTS 18

typedef void MenuFunction();

void ChangeMenu();

struct globals_s {
	team_t hostTeam;
	bool spawnBots, dropCanswap, kickBot;
	int teamTime, classTime, dropTime, clientNum;
};

extern globals_s Globals;

typedef struct slider_s {
	int *value;
	int minValue, maxValue;
} Slider;

typedef struct scale_s {
	float *value;
	float minValue, maxValue, interval;
	const float *color;
} Scale;

typedef struct subMenuOpt_s {
	const char *title;
	bool *boolean;
	Scale scale;
	Slider slider;
	MenuFunction *function;
} SubMenuOpt;

typedef struct subMenu_s {
	const char *title;
	int parent, position, count;
	SubMenuOpt options[MAX_OPTS];
} SubMenu;

class Offhost {
public:
	int menu, position;
	bool init, isOpen;
	CellPadData cellPad;
	const char *infoText;
	float scrollbarPosition;
	float color[4];
	float themeColor[4];
	int currentClient;
	char currentClientName[MAX_NETNAME];

	SubMenu menus[MAX_MENU];

	int32_t StartMenu();
	void CalculateMenuHeight();
	void LoadMenu(usecond_t ms);
	void UpdateMenuHeight(usecond_t ms);
	void MenuTransitionIn(usecond_t ms);
	void MenuTransitionOut(usecond_t ms);
	void BuildMenu();
	void Open();
	void Close();
	void DrawInfoText();
	void DrawMenuText();
	void DrawPlayersMenu();
	void DrawMenuShaders();
	int Scroll(int n);
	int Increment(int n);
	void MoveScrollbar(usecond_t ms);
	void Select();
	void GoBack();

	SubMenu GetCurrentMenu();
	SubMenuOpt GetCurrentOption();

	void AddOption(int menu, const char *title, MenuFunction *function);
	void AddBool(int menu, const char *title, MenuFunction *function, bool *boolean);
	void AddSlider(int menu, const char *title, MenuFunction *function, int *value, int minValue, int maxValue);
	void AddScale(int menu, const char *title, MenuFunction *function, float *value, float minValue, float maxValue, float interval, const float *color);
	void AddMenu(int parent, int menu, const char *title);

	struct s_Variables {
		char gamertag[MAX_NETNAME];
		char flashGamertag[MAX_NETNAME];
		bool flashName;
		bool rainbowTheme, coloredMenus, ghostBG;
		bool sweepingUAV;
		bool preventEnemyForfeit, antiCFG;
		bool showControls;
		int prestige;
		bool autoLoad;
		bool depatchBounces, depatchElevators, minisculeHealth, noFallingDamage;
		int minPlayers, maxPlayers;
		bool forceHost, disableDLC;
	} Variables;
};

extern Offhost Ascension;
extern float MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT;

#endif