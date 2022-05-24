#include <fastmath.h>
#include <string.h>

#include <cell/pad/pad_codes.h>
#include <cell/pad/libpad.h>

#include <sys/ppu_thread.h>
#include <sys/prx.h>

#include "Functions.h"
#include "PS3.h"
#include "Offhost.h"
#include "Hook.h"

SYS_MODULE_INFO( AscensionMW2, 0, 1, 1);
SYS_MODULE_START( AscensionMW2_prx_entry );

void ClientCommand(int clientNum)
{
	gentity_s *ent = &g_entities[clientNum];
	if (ent->client) {
		const char *cmdArgZero = SV_Cmd_Argv(0);

		if (!strcmp(cmdArgZero, "notify")) {
			Cmd_CheckNetNotify_f(clientNum);
		} else if (!strcmp(cmdArgZero, "score")) {
			SendScoreboard(ent);
		} else if (!strcmp(cmdArgZero, "mr")) {
			if (!strcmp(SV_Cmd_Argv(3), "endround")) {
				if (IsHost(clientNum))
					Cmd_MenuResponse_f(ent);
				else if (Ascension.Variables.antiCFG)
					KickClient(clientNum, "'%s' server detected you where trying to end the game, Therefore you have been kicked.", xenonUserData->gamertag);
			} else
				Cmd_MenuResponse_f(ent);
		}
	}
}

void SV_ExecuteClientCommand(client_s *cl, const char *s, int fromOldServer) {
	ucmd_t *u;
	SV_Cmd_TokenizeString(s);

	for (int i = 0; i < SV_Cmd_Argc(); i++) {
		const char *cmdArgv = SV_Cmd_Argv(i);

		if (!strcmp(cmdArgv, "savereplay") || !strcmp(cmdArgv, "say") || !strcmp(cmdArgv, "say_team") || !strcmp(cmdArgv, "follownext") || !strcmp(cmdArgv, "followprev") || !strcmp(cmdArgv, "where") || !strcmp(cmdArgv, "callvote") || !strcmp(cmdArgv, "vote") || !strcmp(cmdArgv, "entitylist") || !strcmp(cmdArgv, "set") || !strcmp(cmdArgv, "seta") || !strcmp(cmdArgv, "toggle") || !strcmp(cmdArgv, "togglep") || !strcmp(cmdArgv, "vstr") || !strcmp(cmdArgv, "bind") || !strcmp(cmdArgv, "bindaxis") || strstr(cmdArgv, "aim_")) {
			if (Ascension.Variables.antiCFG)
				KickClient(cl - svs->clients, "'%s' server detected you where using config mods, Therefore you have been kicked.", xenonUserData->gamertag);
			break;
		}
	}

	for (u = ucmds; u->name; u++) {
		if (!strcmp(SV_Cmd_Argv(0), u->name)) {
			if (!fromOldServer || u->allowFromOldServer)
				u->func(cl);
			break;
		}
	}

	if (!fromOldServer && !u->name && SV_Loaded())
		ClientCommand(cl - svs->clients);
	SV_Cmd_EndTokenizedString();
}

int SV_ClientCommand(client_s *cl, msg_t *msg, int fromOldServer) {
	char strBuf[MAX_STRING_CHARS];

	int seq = MSG_ReadLong(msg);
	const char *s = MSG_ReadString(msg, strBuf, MAX_STRING_CHARS);

	if (cl->lastClientCommand < seq) {
		if (seq <= cl->lastClientCommand + 1) {
			SV_ExecuteClientCommand(cl, s, fromOldServer);
			cl->lastClientCommand = seq;
			sprintf(cl->lastClientCommandString, "%s", s);
			return 1;
		} else {
			SV_DropClient(cl, "EXE_LOSTRELIABLECOMMANDS", true);
			return 0;
		}
	} else
		return 1;
}

int SV_ProcessClientCommands(client_s *cl, msg_t *msg, int fromOldServer, int *lastCommand) {
	*lastCommand = MSG_ReadBits(msg, 3);
	if (*lastCommand != clc_EOF) {
		while (*lastCommand == clc_clientCommand) {
			if (!SV_ClientCommand(cl, msg, fromOldServer) || cl->state == 1)
				return 0;
			*lastCommand = MSG_ReadBits(msg, 3);
			if (*lastCommand == clc_EOF)
				return 1;
		}
	}
	return 1;
}

void CycleColors(uint64_t id) {
	const usecond_t ms = 2000;
	const float buffer = 0.005f;

	int randomR, randomG, randomB;
	float r, g, b;
	float halfStepR, halfStepG, halfStepB;
	float fullStepR, fullStepG, fullStepB;
	
	while (1) {

		if (Ascension.Variables.rainbowTheme) {
			randomR = (random_number() % 255);
			r = static_cast<float>(randomR / 255.f);
			halfStepR = fabs(Ascension.color[0] - r) / ms;
			fullStepR = fabs((static_cast<float>(Ascension.themeColor[0]) - static_cast<float>(randomR)) / ms);

			randomG = (random_number() % 255);
			g = static_cast<float>(randomG / 255.f);
			halfStepG = fabs(Ascension.color[1] - g) / ms;
			fullStepG = fabs((static_cast<float>(Ascension.themeColor[1]) - static_cast<float>(randomG)) / ms);

			randomB = (random_number() % 255);
			b = static_cast<float>(randomB / 255.f);
			halfStepB = fabs(Ascension.color[2] - b) / ms;
			fullStepB = fabs((static_cast<float>(Ascension.themeColor[2]) - static_cast<float>(randomB)) / ms);

			for (usecond_t i = 0; i < ms; i++) {
				if (!Ascension.Variables.rainbowTheme)
					break;

				if (fullStepR >= buffer) {
					if (r > Ascension.color[0]) {
						Ascension.color[0] += halfStepR;
						Ascension.themeColor[0] += fullStepR;
					} else {
						Ascension.color[0] -= halfStepR;
						Ascension.themeColor[0] -= fullStepR;
					}
				}
			
				if (fullStepG >= buffer) {
					if (g > Ascension.color[1]) {
						Ascension.color[1] += halfStepG;
						Ascension.themeColor[1] += fullStepG;
					} else {
						Ascension.color[1] -= halfStepG;
						Ascension.themeColor[1] -= fullStepG;
					}
				}
			
				if (fullStepB >= buffer) {
					if (b > Ascension.color[2]) {
						Ascension.color[2] += halfStepB;
						Ascension.themeColor[2] += fullStepB;
					} else {
						Ascension.color[2] -= halfStepB;
						Ascension.themeColor[2] -= fullStepB;
					}
				}

				Sleep(1);
			}

			if (Ascension.Variables.rainbowTheme) {
				if (fullStepR >= buffer) {
					Ascension.color[0] = r;
					Ascension.themeColor[0] = randomR;
				}

				if (fullStepG >= buffer) {
					Ascension.color[1] = g;
					Ascension.themeColor[1] = randomG;
				}

				if (fullStepB >= buffer) {
					Ascension.color[2] = b;
					Ascension.themeColor[2] = randomB;
				}
			}
		} else {
			Ascension.color[0] = static_cast<float>(Ascension.themeColor[0] / 255.f);
			Ascension.color[1] = static_cast<float>(Ascension.themeColor[1] / 255.f);
			Ascension.color[2] = static_cast<float>(Ascension.themeColor[2] / 255.f);
		}

		Sleep(1);
	}

	sys_ppu_thread_exit(id);
}

void AscensionMW2(uint64_t id) {
	strcpy(xenonUserData->gamertag, Ascension.Variables.gamertag);
	while (1) {
		if (!Ascension.isOpen && Ascension.cellPad.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_R3 && Ascension.cellPad.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_DOWN) {
			Ascension.Open();
			Sleep(150);
		} else if (Ascension.isOpen && Ascension.cellPad.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_CIRCLE) {
			if (Ascension.GetCurrentMenu().parent != -1) {
				Ascension.GoBack();
				Sleep(150);
			} else 
				Ascension.Close();	
		}
		if (Ascension.isOpen) {
			if (Ascension.cellPad.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_UP) {
				Ascension.Scroll(-1);
				Sleep(150);
			}

			if (Ascension.cellPad.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_DOWN) {
				Ascension.Scroll(1);
				Sleep(150);
			}

			if (Ascension.cellPad.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_LEFT) {
				Ascension.Increment(-1);
				if (Ascension.GetCurrentOption().scale.value == NULL)
					Sleep(150);
				else
					Sleep(10);
			}

			if (Ascension.cellPad.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_RIGHT) {
				Ascension.Increment(1);
				if (Ascension.GetCurrentOption().scale.value == NULL)
					Sleep(150);
				else
					Sleep(10);
			}

			if (Ascension.cellPad.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_SQUARE) {
				Ascension.Select();
				Sleep(150);
			}
		}

		Sleep(1);
	}

	sys_ppu_thread_exit(id);
}

// An exported function is needed to generate the project's PRX stub export library
extern "C" int _AscensionMW2_export_function(void)
{
    return CELL_OK;
}

extern "C" int AscensionMW2_prx_entry(void)
{
	Sleep(10000);
	if (Ascension.StartMenu() == SUCCEEDED) {
		Ascension.BuildMenu();

		HookFunction(0x50F7C4, *(uint32_t *)CellPadGetData_Hook);
		HookFunctionStart(0x1689D8, *(uint32_t *)ClientThink_Real_Hook, *(uint32_t *)ClientThink_Real_Stub);
		HookFunctionStart(0x253670, *(uint32_t *)Menu_PaintAll_Internal_Hook, *(uint32_t *)Menu_PaintAll_Internal_Stub);
		HookFunctionStart(0x1AEB90, *(uint32_t *)Scr_PlayerDamage_Internal_Hook, *(uint32_t *)Scr_PlayerDamage_Internal_Stub);
		HookFunctionStart(0x22A000, *(uint32_t *)UI_DrawHandlePic_Hook, *(uint32_t *)UI_DrawHandlePic_Stub);
		HookFunctionStart(0x20B7C8, *(uint32_t *)VM_Notify_Hook, *(uint32_t *)VM_Notify_Stub);
		HookFunction(0x215310, *(uint32_t*)SV_ProcessClientCommands);
		HookFunctionStart(0x210A48, *(uint32_t *)VM_Resume_Hook, *(uint32_t *)VM_Resume_Stub);

		sys_ppu_thread_t AscensionMW2_t;
		sys_ppu_thread_t CycleColors_t;

		sys_ppu_thread_create(&AscensionMW2_t, AscensionMW2, NULL, 1002, 0x7000, 0, "Ascension Thread");
		sys_ppu_thread_create(&CycleColors_t, CycleColors, NULL, 1004, 0x7000, 0, "Menu Theme Thread");
	}

    return SYS_PRX_RESIDENT;
}
