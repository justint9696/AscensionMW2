#include "Dvar.h"
#include "Game.h"

void SetDvar(const char *dvarName, const char *value) {
	Dvar_SetFromStringByName(dvarName, value);
}

void ResetDvar(const char *dvarName) {
	Dvar_Reset(Dvar_FindVar(dvarName), DVAR_SOURCE_EXTERNAL);
}

void SetClientDvar(int clientNum, const char *dvarName, const char *value) {
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, va("v %s \"%s\"", dvarName, value));
}

opd_s Dvar_FindVar_s = { 0x2762C8, TOC };
Dvar_FindVar_t Dvar_FindVar = Dvar_FindVar_t(&Dvar_FindVar_s);

opd_s Dvar_Reset_s = { 0x273518, TOC };
Dvar_Reset_t Dvar_Reset = Dvar_Reset_t(&Dvar_Reset_s);

opd_s Dvar_SetFromStringByName_s = { 0x2767E0, TOC };
Dvar_SetFromStringByName_t Dvar_SetFromStringByName = Dvar_SetFromStringByName_t(&Dvar_SetFromStringByName_s);

opd_s Dvar_GetBool_s = { 0x277138, TOC };
Dvar_GetBool_t Dvar_GetBool = Dvar_GetBool_t(&Dvar_GetBool_s);

opd_s Dvar_GetInt_s = { 0x2770C8, TOC };
Dvar_GetInt_t Dvar_GetInt = Dvar_GetInt_t(&Dvar_GetInt_s);