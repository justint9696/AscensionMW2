#ifndef _OPD_
#define _OPD_

#include "Struct.h"

#define TOC 0x724C38

typedef void(*UI_KeyboardCallback)(int localClientNum, bool ifExecuted, const char* text);

struct opd_s {
	unsigned int sub;
	unsigned int toc;
};

opd_s ParseAddr(uint64_t address);

#define Scr_GetSelf(Value) ((int(*)(int))&ParseAddr(0x201F90))(Value)

#define Scr_AddInt(Value) ((void(*)(int))&ParseAddr(0x20C608))(Value)
#define Scr_AddFloat(Value) ((void(*)(float))&ParseAddr(0x20C5C0))(Value)
#define Scr_AddString(String) ((void(*)(const char *))&ParseAddr(0x20C428))(String)
#define Scr_AddEntity(Entity) ((void(*)(int))&ParseAddr(0x1B7700))(Entity)
#define Scr_AddVector(Vec) ((void(*)(float *))&ParseAddr(0x20C330))(Vec)
#define Scr_AddUndefined() ((void(*)())&ParseAddr(0x20C548))();

#define Scr_ClearOutParams() ((void(*)(void))&ParseAddr(0x20C208))()

#define Scr_GetInt(Value) ((int(*)(int))&ParseAddr(0x20A430))(Value)
#define Scr_GetFloat(Value) ((float(*)(int))&ParseAddr(0x20A340))(Value)
#define Scr_GetString(Value) ((const char*(*)(int))&ParseAddr(0x20AA30))(Value)
#define Scr_GetEntity(Value) ((int(*)(int))&ParseAddr(0x1B7630))(Value)
#define Scr_GetVector(Value1, Value2) ((float*(*)(int, float*))&ParseAddr(0x20B700))(Value1, Value2)

typedef void(__cdecl *Scr_Notify_t)(int *ent, short stringValue, unsigned int paramcount);
extern Scr_Notify_t Scr_Notify;

typedef void(__cdecl *Scr_NotifyNum_t)(int entnum, unsigned int classnum, unsigned int stringValue, unsigned int paramcount);
extern Scr_NotifyNum_t Scr_NotifyNum;

typedef void(__cdecl *SV_GetConfigString_t)(int index, char* buffer, int bufferSize);
extern SV_GetConfigString_t SV_GetConfigString;

typedef int(__cdecl *_vsnprintf_t)(char *s, size_t n, const char *format, va_list arg);
extern _vsnprintf_t _vsnprintf;

typedef char *(__cdecl * va_t)(const char *format, ...);
extern va_t va;

typedef int(__cdecl *Key_IsDown_t)(int localClientNum, int keyNum);
extern Key_IsDown_t Key_IsDown;

typedef ScreenPlacement *(__cdecl *GetScreenPlacement_t)(int localClientNum);
extern GetScreenPlacement_t GetScreenPlacement;

typedef void *(__cdecl *UI_GetFontHandle_t)(ScreenPlacement *scrPlace, int fontEnum, float fontScale);
extern UI_GetFontHandle_t UI_GetFontHandle;

typedef void *(__cdecl *R_RegisterFont_t)(const char *fontName, int imageTrack);
extern R_RegisterFont_t R_RegisterFont;

typedef void(__cdecl *UI_DrawHandlePic_t)(ScreenPlacement *scrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, const float *color, Material *material);
extern UI_DrawHandlePic_t UI_DrawHandlePic;

typedef void(__cdecl *UI_DrawText_t)(ScreenPlacement *scrPlace, const char *text, int maxChars, Font_s *font, float x, float y, int horzAlign, int vertAlign, float fontScale, const float *color, int style);
extern UI_DrawText_t UI_DrawText;

typedef void(__cdecl *UI_DrawTextWithGlow_t)(ScreenPlacement *scrPlace, const char *text, int maxChars, Font_s *font, float x, float y, int horzAlign, int vertAlign, float scale, const float *color, int style, const float *glowColor, int arg1, int arg2, int fxMaterial, int fxMaterialGlow, int fxBirthTime, int fxLetterTime, int fxDecayStartTime, int fxDecayDuration);
extern UI_DrawTextWithGlow_t UI_DrawTextWithGlow;

typedef int(__cdecl *UI_TextWidth_t)(const char *text, int maxChars, Font_s *font, float scale);
extern UI_TextWidth_t UI_TextWidth;

typedef ScreenPlacement *(__cdecl *ScrPlace_GetActivePlacement_t)(int localClientNum);
extern ScrPlace_GetActivePlacement_t ScrPlace_GetActivePlacement;

typedef void(__cdecl *SV_GameSendServerCommand_t)(int clientNum, svscmd_type type, const char *cmd);
extern SV_GameSendServerCommand_t SV_GameSendServerCommand;

typedef Material*(__cdecl *Material_RegisterHandle_t)(const char *shader, int imageTrack);
extern Material_RegisterHandle_t Material_RegisterHandle;

typedef void(__cdecl *OpenKeyboard_t)(int localClientNum, const char *title, const char *presetMessage, size_t size, UI_KeyboardCallback function);
extern OpenKeyboard_t OpenKeyboard;

typedef int(__cdecl *GetLobbyMemberCount_t)();
extern GetLobbyMemberCount_t GetLobbyMemberCount;

typedef int(__cdecl *Party_IsRunning_t)(PartyData_s *party);
extern Party_IsRunning_t Party_IsRunning;

typedef char *(__cdecl *SL_ConvertToString_t)(unsigned int stringValue);
extern SL_ConvertToString_t SL_ConvertToString;

typedef int(__cdecl *SL_GetString_t)(const char *str, unsigned int user);
extern SL_GetString_t SL_GetString;

typedef int(__cdecl *CG_GetClientNum_t)(int localClientNum);
extern CG_GetClientNum_t CG_GetClientNum;

typedef bool(__cdecl *Session_IsHost_t)(SessionData *session, int clientNum);
extern Session_IsHost_t Session_IsHost;

typedef void(__cdecl *ClientCleanName_t)(const char *in, char *out, int outSize);
extern ClientCleanName_t ClientCleanName;

typedef void(__cdecl *UI_OpenMenu_t)(int localClientNum, const char *menuName);
extern UI_OpenMenu_t UI_OpenMenu;

typedef void(__cdecl *CBuf_AddText_t)(int localClientNum, const char* text);
extern CBuf_AddText_t CBuf_AddText;

typedef int(__cdecl *SV_AddTestClient_t)();
extern SV_AddTestClient_t SV_AddTestClient;

typedef int(__cdecl *GSC_AddTestClient_t)();
extern GSC_AddTestClient_t GSC_AddTestClient;

typedef void(__cdecl *SV_Cmd_TokenizeString_t)(const char *text);
extern SV_Cmd_TokenizeString_t SV_Cmd_TokenizeString;

typedef void(__cdecl *SV_Cmd_EndTokenizedString_t)();
extern SV_Cmd_EndTokenizedString_t SV_Cmd_EndTokenizedString;

typedef void(__cdecl *Cmd_CheckNetNotify_f_t)(int clientNum);
extern Cmd_CheckNetNotify_f_t Cmd_CheckNetNotify_f;

typedef void(__cdecl *SendScoreboard_t)(gentity_s *pEnt);
extern SendScoreboard_t SendScoreboard;

typedef void(__cdecl *Cmd_MenuResponse_f_t)(gentity_s *pEnt);
extern Cmd_MenuResponse_f_t Cmd_MenuResponse_f;

typedef bool(__cdecl *SV_Loaded_t)();
extern SV_Loaded_t SV_Loaded;

typedef void(__cdecl *SV_DropClient_t)(client_s *drop, const char *reason, bool tellThem);
extern SV_DropClient_t SV_DropClient;

typedef void(__cdecl *PlayerCmd_dropItem_t)(scr_entref_t entref);
extern PlayerCmd_dropItem_t PlayerCmd_dropItem;

typedef int(__cdecl *G_GetWeaponIndexForName_t)(const char *weaponName);
extern G_GetWeaponIndexForName_t G_GetWeaponIndexForName;

typedef void(__cdecl *PlayerCmd_GiveWeapon_t)(scr_entref_t entref);
extern PlayerCmd_GiveWeapon_t PlayerCmd_GiveWeapon;

typedef int(__cdecl *I_stricmp_t)(const char *str1, const char *str2);
extern I_stricmp_t I_stricmp;

typedef void(__cdecl *G_SetOrigin_t)(gentity_s *ent, const float *origin);
extern G_SetOrigin_t G_SetOrigin;

typedef void(__cdecl *PlayerCmd_setOrigin_t)(scr_entref_t entref);
extern PlayerCmd_setOrigin_t PlayerCmd_setOrigin;

typedef int(__cdecl *MSG_ReadLong_t)(msg_t *msg);
extern MSG_ReadLong_t MSG_ReadLong;

typedef char *(__cdecl *MSG_ReadString_t)(msg_t *msg, char *string, unsigned int maxChars);
extern MSG_ReadString_t MSG_ReadString;

typedef char(__cdecl *BG_GetWeaponModel_t)(playerState_s *ps, unsigned int index);
extern BG_GetWeaponModel_t BG_GetWeaponModel;

typedef int(__cdecl *Drop_Weapon_t)(gentity_s *pEnt, int weaponIndex, char weaponModel, unsigned int tag);
extern Drop_Weapon_t Drop_Weapon;

typedef void(__cdecl *Item_SetDefaultVelocity_t)(gentity_s *pEnt, int dropped);
extern Item_SetDefaultVelocity_t Item_SetDefaultVelocity;

typedef void(__cdecl *G_EntEnablePhysics_t)(gentity_s *pEnt, int physCollmap);
extern G_EntEnablePhysics_t G_EntEnablePhysics;

typedef int(__cdecl *BG_GetWeaponDef_t)(unsigned int weaponIndex);
extern BG_GetWeaponDef_t BG_GetWeaponDef;

typedef void(__cdecl *Item_SetStateFromTag_t)(gentity_s *pEnt, unsigned int tag, int weaponEnt);
extern Item_SetStateFromTag_t Item_SetStateFromTag;

typedef void(__cdecl *G_GivePlayerWeapon_t)(gclient_s *client, int weaponIndex, int camo, int akimbo);
extern G_GivePlayerWeapon_t G_GivePlayerWeapon;

typedef void(__cdecl *G_InitializeAmmo_t)(gentity_s *pEnt, int weaponIndex, int akimbo, int camo);
extern G_InitializeAmmo_t G_InitializeAmmo;

#endif