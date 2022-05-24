#include <string.h>

#include "Hook.h"
#include "Offhost.h"
#include "Functions.h"
#include "Dvar.h"
#include "Handle.h"

int CellPadGetData_Hook(uint32_t port_no, CellPadData *data) {
	CellPadData tmpCellPad;
	tmpCellPad.len = data->len;
	cellPadGetData(port_no, &tmpCellPad);
	memcpy((void *)data, (void *)&tmpCellPad, sizeof(CellPadData));

	if (data->len > 0) {
		memcpy(&Ascension.cellPad, &tmpCellPad, sizeof(CellPadData));
	}

	if (data->len > 0 && Ascension.isOpen) {
		data->button[CELL_PAD_BTN_OFFSET_DIGITAL1] &= ~CELL_PAD_CTRL_R3;
		data->button[CELL_PAD_BTN_OFFSET_DIGITAL1] &= ~CELL_PAD_CTRL_UP;
		data->button[CELL_PAD_BTN_OFFSET_DIGITAL1] &= ~CELL_PAD_CTRL_DOWN;
		data->button[CELL_PAD_BTN_OFFSET_DIGITAL1] &= ~CELL_PAD_CTRL_LEFT;
		data->button[CELL_PAD_BTN_OFFSET_DIGITAL1] &= ~CELL_PAD_CTRL_RIGHT;
		data->button[CELL_PAD_BTN_OFFSET_DIGITAL2] &= ~CELL_PAD_CTRL_SQUARE;
		data->button[CELL_PAD_BTN_OFFSET_DIGITAL2] &= ~CELL_PAD_CTRL_CIRCLE;
	}

	return 0;
}

void ClientThink_Real_Hook(gentity_s *ent, usercmd_s *ucmd) {
	ExecuteEvents();
	if (IsInGame() && IsServerHost()) {
		if (Ascension.Variables.preventEnemyForfeit) {
			if (PreventEnemyForfeit())
				Globals.spawnBots = true;
			else if (!IsPrivateMatch() && !GetBotClientNum())
				Globals.kickBot = true;
		}
		if (Globals.spawnBots) {
			SpawnBot();
			Globals.spawnBots = false;
		}
		if (Globals.dropCanswap) {
			DropCanswap();
			Globals.dropCanswap = false;
		}
		if (Globals.kickBot) {
			KickClient(GetBotClientNum(), NULL);
			Globals.kickBot = false;
		}
	}

	ClientThink_Real_Stub(ent, ucmd);
}

void ClientThink_Real_Stub(gentity_s* ent, usercmd_s *ucmd) {
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
}

void Menu_PaintAll_Internal_Hook(UiContext *dc, int init) {
	Ascension.DrawInfoText();

	if (Ascension.isOpen) {
		Ascension.DrawMenuShaders();
		if (Ascension.menu == 6)
			Ascension.DrawPlayersMenu();
		else
			Ascension.DrawMenuText();
	}

	if (IsOnlineGame() && IsPublicLobbyActive() && Ascension.Variables.forceHost) {
		SetDvar("party_connectToOthers", "0");
		SetDvar("party_host", "1");
		SetDvar("party_hostmigration", "0");
		SetDvar("party_matchedPlayerCount", "2");
		SetDvar("party_minPlayers", va("%i", Ascension.Variables.minPlayers));
	}

	Menu_PaintAll_Internal_Stub(dc, init);
}

void Menu_PaintAll_Internal_Stub(UiContext *dc, int init) {
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
}

void Scr_PlayerDamage_Internal_Hook(gentity_s *victim, gentity_s *eInflictor, gentity_s *eAttacker, int iDamage, int iDFlags, int sMeansOfDeath, int iWeapon, const float *vPoint, const float *vDir, int sHitLoc, int psOffsetTime) {
	if (iDamage > 1000 && iDFlags == 2 && (sMeansOfDeath == 9 || sMeansOfDeath == 2))
		return; 

	if (Ascension.Variables.noFallingDamage && (sMeansOfDeath == 11 || sMeansOfDeath == 14) && victim->client->sess.cs.team == Globals.hostTeam)
		return;

	if (eAttacker && eInflictor && victim != eInflictor && victim->client && eInflictor->client && eAttacker->client) {
		if (Ascension.Variables.minisculeHealth && iDamage != 2) {
			bool ffa = eInflictor->client->sess.cs.team == 0;
			if (ffa || victim->client->sess.cs.team != Globals.hostTeam)
				iDamage = 1000;
			else
				iDamage -= (iDamage / 4);
		}
	}

	Scr_PlayerDamage_Internal_Stub(victim, eInflictor, eAttacker, iDamage, iDFlags, sMeansOfDeath, iWeapon, vPoint, vDir, sHitLoc, psOffsetTime);
}

void Scr_PlayerDamage_Internal_Stub(gentity_s *victim, gentity_s *eInflictor, gentity_s *eAttacker, int iDamage, int iDFlags, int sMeansOfDeath, int iWeapon, const float *vPoint, const float *vDir, int sHitLoc, int psOffsetTime) {
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
}

void UI_DrawHandlePic_Hook(ScreenPlacement *scrPlace, float x, float y, float width, float height, int horzAlign, int vertAlign, const float *color, Material *material) {
	if (Ascension.Variables.ghostBG) {
		if (!memcmp(material, GetMaterial("mw2_main_mp_image"), sizeof(material)) || !memcmp(material, GetMaterial("mw2_main_background"), sizeof(material)))
			material = GetMaterial("mw2_main_sp_image");
	}

	if (!memcmp(material, GetMaterial("mockup_bg_glow"), sizeof(material))) {
		memcpy((void *)color, Ascension.color, sizeof(float) * 3);
	}

	if (Ascension.Variables.coloredMenus) {
		if (!memcmp(material, GetMaterial("mw2_main_cloud_overlay"), sizeof(material)))
			memcpy((void *)color, Ascension.color, sizeof(float) * 3);
		if (!Ascension.Variables.ghostBG) {
			if (!memcmp(material, GetMaterial("mw2_main_background"), sizeof(material)))
				memcpy((void *)color, Ascension.color, sizeof(float) * 3);
		}
	}

	UI_DrawHandlePic_Stub(scrPlace, x, y, width, height, horzAlign, vertAlign, color, material);
}

void UI_DrawHandlePic_Stub(ScreenPlacement *scrPlace, float x, float y, float width, float height, int horzAlign, int vertAlign, const float *color, Material *material) {
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
}

void VM_Notify_Hook(unsigned int notifyListOwnerId, unsigned short stringValue, VariableValue *top) {
	int clientNum = Scr_GetSelf(notifyListOwnerId);
	if (SV_IsPlayer(clientNum)) {
		if (!strcmp(SL_ConvertToString(stringValue), "begin"))
			OnPlayerBegin(clientNum);
		else if (!strcmp(SL_ConvertToString(stringValue), "spawned_player"))
			OnPlayerSpawned(clientNum);
	}
	VM_Notify_Stub(notifyListOwnerId, stringValue, top);
}

void VM_Notify_Stub(unsigned int notifyListOwnerId, unsigned short stringValue, VariableValue *top) {
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
}

void VM_Resume_Hook(int timeId) {
	VM_Resume_Stub(timeId);
}

void VM_Resume_Stub(int timeId) {
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
}