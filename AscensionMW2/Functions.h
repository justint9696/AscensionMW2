#ifndef _FUNCTIONS_
#define _FUNCTIONS_

#include "Struct.h"
#include "Game.h"

extern sys_ppu_thread_t FlashName_t;

extern "C" {
    int _sys_printf(const char *format, ...);
    int _sys_sprintf(char *s, const char *format, ...);
    int _sys_vsprintf(char *s, const char *format, va_list arg);
    int _sys_vsnprintf(char *s, size_t n, const char *format, va_list arg);

    void *_sys_malloc(size_t n);
    void _sys_free(void *ptr);
}

#define printf _sys_printf
#define sprintf _sys_sprintf
#define vsprintf _sys_vsprintf

#define HIWORD(a) ((short)(((int)(a) >> 16) & 0xFFFF))

float GetTextWidth(const char *text, const char *font, float scale);
Material *GetMaterial(const char *shader);
ScreenPlacement *GetActiveScreenPlacement();
float CalculateCenterWidthAdjustment(float width);
float CalculateCenterHeightAdjustment(float height);
void Test();
void ChangeNameKeyboardCallback(int localClientNum, bool ifExecuted, const char* text);
void DrawKeyboard(const char *title, const char *presetMessage, size_t size, uint32_t panelMode, UI_KeyboardCallback function);
void ChangeName();
void ResetName();
void SetLevel70();
void SetPrestige();
void UnlockAll();
void ToggleBounces();
void ToggleElevators();
void ToggleDLC();
bool Content_IsDLC1Installed();
bool Content_IsDLC2Installed();
bool Content_IsAnyDLCPackInstalled();
int Content_GetInstalledDLCContentPackIndex();
bool IsInGame();
bool IsOnlineGame();
bool IsPrivateMatch();
bool IsHost(int clientNum);
bool SV_IsPlayer(int entNum);
int GetMaxClients();
int GetSelfClientNum();
void ToggleForceHost();
bool IsPublicLobbyActive();
bool IsServerHost();
void SetLobbySize(const char *party_maxPlayers);
void UpdateLobbySize();
void SaveStats();
void LoadStats();
bool FileExists(const char *filename);
bool ReadFile(const char *filename, char *buffer, size_t size);
bool WriteFile(const char *filename, char *buffer, size_t size);
void OnPlayerBegin(int clientNum);
void OnPlayerSpawned(int clientNum);
gclient_s *G_Client(int clientNum);
gentity_s *G_Entity(int clientNum);
const char *SV_GetClientName(int clientNum);
bool SV_IsClientActive(int clientNum);
bool SV_IsTestClient(int clientNum);
void ToggleFlashName();
void OpenMapSelectMenu();
void OpenGamemodeSelectMenu();
void FlashName(uint64_t id);
void PreventForfeit();
void SpawnBot();
void Cmd_MenuResponse(int ent, int MenuIndex, char* Value);
void SetName(int botNum);
void TeamSelect(int botNum);
void ClassSelect(int botNum);
void PreventForfeit();
bool PreventEnemyForfeit();
void Scr_SetNumParam(int num);
void KickClient();
void KickClient(int clientNum, const char *reasonFormat, ...);
void UnlockAllOne(int clientNum);
void UnlockAllTwo(int clientNum);
void UnlockAllThree(int clientNum);
void UnlockAllFour(int clientNum);
void UnlockAllFive(int clientNum);
void UnlockAllClient();
void FastRestart();
void GiveWeapon(int clientNum, const char *weaponName, int camo, bool akimbo);
void DropItem(int clientNum, const char *weaponName);
void DropCanswap();
int GetLevelTime();
void MoveBot();
void SetUAV(int clientNum, bool hasRadar);
void ConstantUAV();
int GetBotClientNum();
void ChangeBackground();
void Inject(Material *material, const char *imagePath);
int SV_Cmd_Argc();
const char * SV_Cmd_Argv(int argIndex);
int MSG_GetByte(msg_t *msg, int where);
int MSG_ReadBits(msg_t *msg, int bits);
void FreezeControls(int clientNum, bool freeze);
void iPrintln(int clientNum, const char * format, ...);
void iPrintlnBold(int clientNum, const char * format, ...);

#endif