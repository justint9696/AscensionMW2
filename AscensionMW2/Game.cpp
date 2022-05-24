#include "Game.h"

opd_s ParseAddr(uint64_t address) {
	opd_s GLS = { address, TOC };
	return GLS;
}

opd_s Scr_Notify_s = { 0x1B74F0, TOC };
Scr_Notify_t Scr_Notify = Scr_Notify_t(&Scr_Notify_s);

opd_s Scr_NotifyNum_s = { 0x20CBC0 , TOC };
Scr_NotifyNum_t Scr_NotifyNum = Scr_NotifyNum_t(&Scr_NotifyNum_s);

opd_s SV_GetConfigString_s = { 0x21B7B0, TOC };
SV_GetConfigString_t SV_GetConfigString = SV_GetConfigString_t(&SV_GetConfigString_s);

opd_s _vsnprintf_s = { 0x4C6C58, TOC };
_vsnprintf_t _vsnprintf = _vsnprintf_t(&_vsnprintf_s);

opd_s va_s = { 0x27E0D0, TOC };
va_t va = va_t(&va_s);

opd_s Key_IsDown_s = { 0xA4FF0, TOC };
Key_IsDown_t Key_IsDown = Key_IsDown_t(&Key_IsDown_s);

opd_s GetScreenPlacement_s = { 0xDB9B0, TOC };
GetScreenPlacement_t GetScreenPlacement = GetScreenPlacement_t(&GetScreenPlacement_s);

opd_s UI_GetFontHandle_s = { 0x239D88, TOC };
UI_GetFontHandle_t UI_GetFontHandle = UI_GetFontHandle_t(&UI_GetFontHandle_s);

opd_s R_RegisterFont_s = { 0xA8510, TOC };
R_RegisterFont_t R_RegisterFont = R_RegisterFont_t(&R_RegisterFont_s);

opd_s UI_DrawHandlePic_s = { 0x22A000, TOC };
UI_DrawHandlePic_t UI_DrawHandlePic = UI_DrawHandlePic_t(&UI_DrawHandlePic_s);

opd_s UI_DrawText_s = { 0x23A138, TOC };
UI_DrawText_t UI_DrawText = UI_DrawText_t(&UI_DrawText_s);

opd_s UI_DrawTextWithGlow_s = { 0x23A340, TOC };
UI_DrawTextWithGlow_t UI_DrawTextWithGlow = UI_DrawTextWithGlow_t(&UI_DrawTextWithGlow_s);

opd_s UI_TextWidth_s = { 0x23B1F8, TOC };
UI_TextWidth_t UI_TextWidth = UI_TextWidth_t(&UI_TextWidth_s);

opd_s ScrPlace_GetActivePlacement_s = { 0xDB9B0, TOC };
ScrPlace_GetActivePlacement_t ScrPlace_GetActivePlacement = ScrPlace_GetActivePlacement_t(&ScrPlace_GetActivePlacement_s);

opd_s SV_GameSendServerCommand_s = { 0x21A0A0, TOC };
SV_GameSendServerCommand_t SV_GameSendServerCommand = SV_GameSendServerCommand_t(&SV_GameSendServerCommand_s);

opd_s Material_RegisterHandle_s = { 0x4C6D28, TOC };
Material_RegisterHandle_t Material_RegisterHandle = Material_RegisterHandle_t(&Material_RegisterHandle_s);

opd_s OpenKeyboard_s = { 0x238070, TOC };
OpenKeyboard_t OpenKeyboard = OpenKeyboard_t(&OpenKeyboard_s);

opd_s GetLobbyMemberCount_s = { 0x2415E8, TOC };
GetLobbyMemberCount_t GetLobbyMemberCount = GetLobbyMemberCount_t(&GetLobbyMemberCount_s);

opd_s Party_IsRunning_s = { 0xC71B8, TOC };
Party_IsRunning_t Party_IsRunning = Party_IsRunning_t(&Party_IsRunning_s);

opd_s SL_ConvertToString_s = { 0x200280, TOC };
SL_ConvertToString_t SL_ConvertToString = SL_ConvertToString_t(&SL_ConvertToString_s);

opd_s SL_GetString_s = { 0x2013A8, TOC };
SL_GetString_t SL_GetString = SL_GetString_t(&SL_GetString_s);

opd_s CG_GetClientNum_s = { 0x610F0, TOC };
CG_GetClientNum_t CG_GetClientNum = CG_GetClientNum_t(&CG_GetClientNum_s);

opd_s Session_IsHost_s = { 0x2F19A8, TOC };
Session_IsHost_t Session_IsHost = Session_IsHost_t(&Session_IsHost_s);

opd_s ClientCleanName_s = { 0x16C450, TOC };
ClientCleanName_t ClientCleanName = ClientCleanName_t(&ClientCleanName_s);

opd_s UI_OpenMenu_s = { 0x236378, TOC };
UI_OpenMenu_t UI_OpenMenu = UI_OpenMenu_t(&UI_OpenMenu_s);

opd_s CBuf_AddText_s = { 0x1D9EC0, TOC };
CBuf_AddText_t CBuf_AddText = CBuf_AddText_t(&CBuf_AddText_s);

opd_s SV_AddTestClient_s = { 0x2189D8, TOC };
SV_AddTestClient_t SV_AddTestClient = SV_AddTestClient_t(&SV_AddTestClient_s); 

opd_s GSC_AddTestClient_s = { 0x1ACBE8, TOC };
GSC_AddTestClient_t GSC_AddTestClient = GSC_AddTestClient_t(&GSC_AddTestClient_s);

opd_s SV_Cmd_TokenizeString_s = { 0x1DB040, TOC };
SV_Cmd_TokenizeString_t SV_Cmd_TokenizeString = SV_Cmd_TokenizeString_t(&SV_Cmd_TokenizeString_s);

opd_s SV_Cmd_EndTokenizedString_s = { 0x1D9270, TOC };
SV_Cmd_EndTokenizedString_t SV_Cmd_EndTokenizedString = SV_Cmd_EndTokenizedString_t(&SV_Cmd_EndTokenizedString_s);

opd_s Cmd_CheckNetNotify_f_s = { 0x1DA520, TOC };
Cmd_CheckNetNotify_f_t Cmd_CheckNetNotify_f = Cmd_CheckNetNotify_f_t(&Cmd_CheckNetNotify_f_s);

opd_s SendScoreboard_s = { 0x17AA30, TOC };
SendScoreboard_t SendScoreboard = SendScoreboard_t(&SendScoreboard_s);

opd_s Cmd_MenuResponse_f_s = { 0x179868, TOC };
Cmd_MenuResponse_f_t Cmd_MenuResponse_f = Cmd_MenuResponse_f_t(&Cmd_MenuResponse_f_s);

opd_s SV_Loaded_s = { 0x21A770, TOC };
SV_Loaded_t SV_Loaded = SV_Loaded_t(&SV_Loaded_s);

opd_s SV_DropClient_s = { 0x215000, TOC };
SV_DropClient_t SV_DropClient = SV_DropClient_t(&SV_DropClient_s);

opd_s PlayerCmd_dropItem_s = { 0x1780A0, TOC };
PlayerCmd_dropItem_t PlayerCmd_dropItem = PlayerCmd_dropItem_t(&PlayerCmd_dropItem_s);

opd_s G_GetWeaponIndexForName_s = { 0x1BF4A0, TOC };
G_GetWeaponIndexForName_t G_GetWeaponIndexForName = G_GetWeaponIndexForName_t(&G_GetWeaponIndexForName_s);

opd_s PlayerCmd_GiveWeapon_s = { 0x175370, TOC };
PlayerCmd_GiveWeapon_t PlayerCmd_GiveWeapon = PlayerCmd_GiveWeapon_t(&PlayerCmd_GiveWeapon_s);

opd_s I_stricmp_s = { 0x27CC40, TOC };
I_stricmp_t I_stricmp = I_stricmp_t(&I_stricmp_s);

opd_s G_SetOrigin_s = { 0x1BC200, TOC };
G_SetOrigin_t G_SetOrigin = G_SetOrigin_t(&G_SetOrigin_s);

opd_s PlayerCmd_setOrigin_s = { 0x1728B0, TOC };
PlayerCmd_setOrigin_t PlayerCmd_setOrigin = PlayerCmd_setOrigin_t(&PlayerCmd_setOrigin_s);

opd_s MSG_ReadLong_s = { 0x1ECDF0, TOC };
MSG_ReadLong_t MSG_ReadLong = MSG_ReadLong_t(&MSG_ReadLong_s);

opd_s MSG_ReadString_s = { 0x1ED388, TOC };
MSG_ReadString_t MSG_ReadString = MSG_ReadString_t(&MSG_ReadString_s);

opd_s BG_GetWeaponModel_s = { 0x33540, TOC };
BG_GetWeaponModel_t BG_GetWeaponModel = BG_GetWeaponModel_t(&BG_GetWeaponModel_s);

opd_s Drop_Weapon_s = { 0x1861C8, TOC };
Drop_Weapon_t Drop_Weapon = Drop_Weapon_t(&Drop_Weapon_s);

opd_s Item_SetDefaultVelocity_s = { 0x184410, TOC };
Item_SetDefaultVelocity_t Item_SetDefaultVelocity = Item_SetDefaultVelocity_t(&Item_SetDefaultVelocity_s);

opd_s G_EntEnablePhysics_s = { 0x1859A8, TOC };
G_EntEnablePhysics_t G_EntEnablePhysics = G_EntEnablePhysics_t(&G_EntEnablePhysics_s);

opd_s BG_GetWeaponDef_s = { 0x32898, TOC };
BG_GetWeaponDef_t BG_GetWeaponDef = BG_GetWeaponDef_t(&BG_GetWeaponDef_s);

opd_s Item_SetStateFromTag_s = { 0x184378, TOC };
Item_SetStateFromTag_t Item_SetStateFromTag = Item_SetStateFromTag_t(&Item_SetStateFromTag_s);

opd_s G_GivePlayerWeapon_s = { 0x1C0890, TOC };
G_GivePlayerWeapon_t G_GivePlayerWeapon = G_GivePlayerWeapon_t(&G_GivePlayerWeapon_s);

opd_s G_InitializeAmmo_s = { 0x174BF8, TOC };
G_InitializeAmmo_t G_InitializeAmmo = G_InitializeAmmo_t(&G_InitializeAmmo_s);