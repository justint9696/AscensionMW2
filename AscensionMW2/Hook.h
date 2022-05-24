#ifndef _HOOK_
#define _HOOK_

#include <sys/types.h>
#include <cell/pad/pad_codes.h>
#include <cell/pad/libpad.h>

#include "Struct.h"

int CellPadGetData_Hook(uint32_t port_no, CellPadData *data);

void ClientThink_Real_Hook(gentity_s *ent, usercmd_s *ucmd);
void ClientThink_Real_Stub(gentity_s *ent, usercmd_s *ucmd);

void Menu_PaintAll_Internal_Stub(UiContext *dc, int init);
void Menu_PaintAll_Internal_Hook(UiContext *dc, int init);

void Scr_PlayerDamage_Internal_Hook(gentity_s *victim, gentity_s *eInflictor, gentity_s *eAttacker, int iDamage, int iDFlags, int sMeansOfDeath, int iWeapon, const float *vPoint, const float *vDir, int sHitLoc, int psOffsetTime);
void Scr_PlayerDamage_Internal_Stub(gentity_s *victim, gentity_s *eInflictor, gentity_s *eAttacker, int iDamage, int iDFlags, int sMeansOfDeath, int iWeapon, const float *vPoint, const float *vDir, int sHitLoc, int psOffsetTime);

void SV_ExecuteClientCommand_Hook(client_s *cl, const char *s, int fromOldServer);
void SV_ExecuteClientCommand_Stub(client_s *cl, const char *s, int fromOldServer);

void UI_DrawHandlePic_Stub(ScreenPlacement *scrPlace, float x, float y, float width, float height, int horzAlign, int vertAlign, const float *color, void *material);
void UI_DrawHandlePic_Hook(ScreenPlacement *scrPlace, float x, float y, float width, float height, int horzAlign, int vertAlign, const float *color, void *material);

void VM_Notify_Hook(unsigned int notifyListOwnerId, unsigned short stringValue, VariableValue *top);
void VM_Notify_Stub(unsigned int notifyListOwnerId, unsigned short stringValue, VariableValue *top);

void VM_Resume_Hook(int timeId);
void VM_Resume_Stub(int timeId);

#endif