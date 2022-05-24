#include <string.h>
#include <sysutil/sysutil_oskdialog.h>
#include <cell.h>

#include "Functions.h"
#include "Struct.h"
#include "Offhost.h"
#include "PS3.h"
#include "Dvar.h"
#include "Handle.h"

sys_ppu_thread_t FlashName_t;

float GetTextWidth(const char *text, const char *font, float scale) {
	return UI_TextWidth(text, 0, R_RegisterFont(font, 0), scale);
}

Material *GetMaterial(const char *shader) {
	return Material_RegisterHandle(shader, 0);
}

ScreenPlacement *GetActiveScreenPlacement() {
	return ScrPlace_GetActivePlacement(0);
}

float CalculateCenterWidthAdjustment(float width){
	return (SCREEN_WIDTH - width) / 2;
}

float CalculateCenterHeightAdjustment(float height) {
	return (SCREEN_HEIGHT - height) / 2;
}

void Test() {}

void ChangeNameKeyboardCallback(int localClientNum, bool ifExecuted, const char* text) {
	if (ifExecuted && strlen(text)) {
		if (Ascension.Variables.gamertag[0] == '\0')
			strcpy(Ascension.Variables.gamertag, xenonUserData->gamertag);
		strcpy(xenonUserData->gamertag, text);
	}
}

void DrawKeyboard(const char *title, const char *presetMessage, size_t size, uint32_t panelMode, UI_KeyboardCallback function) {
	WriteProcessMemory<uint16_t>(0x2F7BDA, HIWORD(panelMode), sizeof(uint16_t));
	OpenKeyboard(0, title, presetMessage, size, function);
	WriteProcessMemory<uint16_t>(0x2F7BDA, HIWORD(CELL_OSKDIALOG_PANELMODE_ALPHABET), sizeof(uint16_t));
}

void ChangeName() {
	Ascension.Close();
	DrawKeyboard("Enter a new name:", "", 0x32, CELL_OSKDIALOG_PANELMODE_ALPHABET, ChangeNameKeyboardCallback);
}

void ResetName() {
	if (Ascension.Variables.gamertag[0] != '\0')
		strcpy(xenonUserData->gamertag, Ascension.Variables.gamertag);
}

void SetLevel70() {
	char xp[] = { 0x20, 0x64, 0x26 };
	memcpy(&stats->xp, &xp, sizeof(xp));
}

void SetPrestige() {
	char prestige = Ascension.Variables.prestige;
	memset(&stats->prestige, prestige, sizeof(int8_t));
}

void UnlockAll() {
	for (int i = 0; i < 480; i++)
		WriteProcessMemory<uint8_t>(0x1FFA0C7 + i, 0xFF, sizeof(uint8_t));

	WriteProcessMemory<uint8_t>(0x1FFAC09, 0xFF, sizeof(uint8_t));
}

void ToggleBounces() {
	if (Ascension.Variables.depatchBounces)
		WriteProcessMemory<uint8_t>(0x2ACFB, 0x1, sizeof(uint8_t));
	else
		WriteProcessMemory<uint8_t>(0x2ACFB, 0x0, sizeof(uint8_t));
}

void ToggleElevators() {
	if (Ascension.Variables.depatchElevators)
		WriteProcessMemory<uint32_t>(0x2433C, 0x60000000, sizeof(uint32_t));
	else
		WriteProcessMemory<uint32_t>(0x2433C, 0x409EFF2C, sizeof(uint32_t));
}

void ToggleDLC() {
	if (Ascension.Variables.disableDLC)
		contentPack->contentPackage = 1;
	else
		contentPack->contentPackage = Content_GetInstalledDLCContentPackIndex();
}

bool Content_IsDLC1Installed() {
	return !strcmp(contentPack->dlcName1, "DLC_1") || !strcmp(contentPack->dlcName2, "DLC_1");
}

bool Content_IsDLC2Installed() {
	return !strcmp(contentPack->dlcName1, "DLC_2") || !strcmp(contentPack->dlcName2, "DLC_2");
}

bool Content_IsAnyDLCPackInstalled() {
	return Content_IsDLC1Installed() || Content_IsDLC2Installed();
}

int Content_GetInstalledDLCContentPackIndex() {
	if (Content_IsDLC1Installed() && Content_IsDLC2Installed())
		return 3;
	else if (Content_IsDLC1Installed() || Content_IsDLC2Installed())
		return 2;
	else
		return 1;
}

bool IsInGame() {
	return Dvar_GetBool("cl_ingame");
}

bool IsOnlineGame() {
	return Dvar_GetBool("onlinegame");
}

bool IsPrivateMatch() {
	return Dvar_GetBool("xblive_privatematch");
}

int GetMaxClients() {
	return Dvar_GetInt("sv_maxclients");
}

int GetSelfClientNum() {
	return CG_GetClientNum(0);
}

bool IsServerHost() {
	return Session_IsHost(g_serverSession, GetSelfClientNum());
}

bool IsHost(int clientNum) {
	return Session_IsHost(g_serverSession, clientNum);
}

bool SV_IsPlayer(int entNum) {
	return g_entities[entNum].client;
}

void ToggleForceHost() {
	if (Ascension.Variables.forceHost) {
		SetDvar("ping_increment", "1");
		SetDvar("ping_default_min", "20");
		SetDvar("party_connectTimeout", "1");
		SetDvar("migration_msgtimeout", "0");
		SetDvar("migration_timeBetween", "999999");
		SetDvar("migration_verboseBroadcastTime", "0");
		SetDvar("lobby_partySearchWaitTime", "0");
		SetDvar("g_kickHostIfIdle", "0");
		SetDvar("badhost_endGameIfISuck", "0");
		SetDvar("badhost_maxDoISuckFrames", "0");
		SetDvar("badhost_maxHappyPingTime", "999");
		SetDvar("badhost_minTotalClientsForHappyTest", "18");
	} else {
		ResetDvar("ping_default_min");
		ResetDvar("ping_increment");
		ResetDvar("party_connectToOthers");
		ResetDvar("party_connectTimeout");
		ResetDvar("party_host");
		ResetDvar("party_hostmigration");
		ResetDvar("migration_msgtimeout");
		ResetDvar("migration_timeBetween");
		ResetDvar("migrationPingTime");
		ResetDvar("lobby_partySearchWaitTime");
		ResetDvar("g_kickHostIfIdle");
		ResetDvar("badhost_endGameIfISuck");
		ResetDvar("badhost_maxDoISuckFrames");
		ResetDvar("badhost_maxHappyPingTime");
		ResetDvar("badhost_minTotalClientsForHappyTest");
	}
}

bool IsPublicLobbyActive() {
	return Party_IsRunning(g_lobbyData) && !IsPrivateMatch();
}

void SetLobbySize(const char *maxPlayers) {
	strcpy(reinterpret_cast<char *>(0x1CA30BD), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CA5AFD), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CA856E), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CAAF7D), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CAD9D7), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CB03FD), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CB2E3D), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CB587D), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CB82BD), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CBAD12), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CBD73C), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CC017D), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CC2BDA), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CC56B1), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CCAA93), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CCD4BD), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CCFEFD), maxPlayers);
	strcpy(reinterpret_cast<char *>(0x1CD293D), maxPlayers);
}

void UpdateLobbySize() {
	char maxPlayers[MAX_STRING_CHARS];
	sprintf(maxPlayers, "; party_maxPlayers %i", Ascension.Variables.maxPlayers);
	SetLobbySize(maxPlayers);
}

void SaveStats() {
	const char *filename = ASSETS_PATH"Stats.bin";
	if (FileExists(filename)) {
		if (DrawYesNoMessageDialog("Do you want to override current stats?")) {
			Sleep(100);
			if (WriteFile(filename, reinterpret_cast<char *>(0x1FF9A00), 0x2000))
				DrawOkayMessageDialog("Stats saved successfully");
			else
				DrawOkayMessageDialog("ERROR: Could not save stats");
		}
	} else if (WriteFile(filename, reinterpret_cast<char *>(0x1FF9A00), 0x2000))
		DrawOkayMessageDialog("Stats saved successfully");
}

void LoadStats() {
	const char *filename = ASSETS_PATH"Stats.bin";
	if (ReadFile(filename, reinterpret_cast<char *>(0x1FF9A00), 0x2000)) 
		DrawOkayMessageDialog("Stats loaded successfully");
	else
		DrawOkayMessageDialog("ERROR: Could not load stats");
}


bool FileExists(const char *filename) {
	int fd = 0;
	if (cellFsOpen(filename, 0, &fd, NULL, 0) != CELL_FS_SUCCEEDED)
		return false;

	cellFsClose(fd);

	return true;
}

bool ReadFile(const char *filename, char *buffer, size_t size) {
	int fd = 0;
	uint64_t pos = 0, nread = 0;
	if (cellFsOpen(filename, 0, &fd, NULL, 0) != CELL_FS_SUCCEEDED)
		return false;

	cellFsLseek(fd, 0, CELL_FS_SEEK_SET, &pos);
	cellFsRead(fd, buffer, size, &nread);
	cellFsClose(fd);

	return true;
}

bool WriteFile(const char *filename, char *buffer, size_t size) {
	int fd = 0;
	uint64_t pos = 0, nwrite = 0;
	if (cellFsOpen(filename, CELL_FS_O_WRONLY | CELL_FS_O_CREAT | CELL_FS_O_TRUNC, &fd, NULL, 0) != CELL_FS_SUCCEEDED)
		return false;

	cellFsLseek(fd, 0, CELL_FS_SEEK_SET, &pos);
	cellFsWrite(fd, buffer, size, &nwrite);
	cellFsClose(fd);

	return true;
}

void OnPlayerBegin(int clientNum) {
	if (IsServerHost()) {
		SetDvar("testClients_watchKillcam", "0");
		SetDvar("testclients_doReload", "0");
		SetDvar("testclients_doMove", "0");
		SetDvar("testclients_doAttack", "0");
		SetDvar("testclients_doCrouch", "0");
	}
	SetClientDvar(clientNum, "loc_warnings", "0");
	SetClientDvar(clientNum, "loc_warningsUI", "0");
}

void OnPlayerSpawned(int clientNum) {
	gclient_s *client = &g_clients[clientNum];
	if (Ascension.Variables.sweepingUAV)
		SetUAV(clientNum, true);
	if (IsHost(clientNum)) {
		Globals.hostTeam = client->sess.cs.team;
		PreventForfeit();
	} else if (SV_IsTestClient(clientNum))
		SetName(clientNum);
}

gclient_s *G_Client(int clientNum) {
	return reinterpret_cast<gclient_s *>(0x14E2200 + (clientNum * 0x3700));
}

gentity_s *G_Entity(int clientNum) {
	return reinterpret_cast<gentity_s *>(0x1319800 + (clientNum * 0x280));
}

const char *SV_GetClientName(int clientNum) {
	if (IsInGame() && cg != NULL) {
		const char *clientName = cg->info[clientNum].name;
		if (strlen(clientName))
			return clientName;
		else
			return "Empty Slot";
	} else
		return "Not In Game";
}

bool SV_IsClientActive(int clientNum) {
	return svs->clients[clientNum].state == CS_ACTIVE;
}

bool SV_IsTestClient(int clientNum) {
	return svs->clients[clientNum].testClient == TC_TEST_CLIENT;
}

void ToggleFlashName() {
	if (Ascension.Variables.flashName) {
		strcpy(Ascension.Variables.flashGamertag, xenonUserData->gamertag);
		sys_ppu_thread_create(&FlashName_t, FlashName, NULL, 1006, 0x7000, 0, "AscensionMW2");
	} else
		strcpy(xenonUserData->gamertag, Ascension.Variables.flashGamertag);
}

void OpenMapSelectMenu() {
	Ascension.Close();
	CBuf_AddText(0, "openmenu settings_map");
}

void OpenGamemodeSelectMenu() {
	Ascension.Close();
	CBuf_AddText(0, "openmenu settings_gametype");
}

void FlashName(uint64_t id) {
	int color = 0;
	char flashName[32];
	while (Ascension.Variables.flashName) {
		sprintf(flashName, "^%i%s", color, Ascension.Variables.flashGamertag);
		strcpy(xenonUserData->gamertag, flashName);
		color = (color + 1) % 7;
		Sleep(500);
	}

	sys_ppu_thread_exit(id);
}

void Cmd_MenuResponse(int ent, int MenuIndex, char *Value) {
	char buffer[100];
	SV_GetConfigString((0x96F + MenuIndex), buffer, 100);
	Scr_AddString(Value);
	Scr_AddString(buffer);
	Scr_Notify((int *)ent, *(short *)(0x173D180 + 0x1E0), 2);
}

void TeamSelect(int botNum) {
	Scr_AddString("autoassign");
	Scr_AddString("team_marinesopfor");
	Scr_NotifyNum(botNum, 0, SL_GetString("menuresponse", 0), 2);
}

void ClassSelect(int botNum) {
	Scr_AddString("class1");
	Scr_AddString("changeclass");
	Scr_NotifyNum(botNum, 0, SL_GetString("menuresponse", 0), 2);
}

void SetName(int botNum) {
	strcpy(g_clients[botNum].sess.cs.name, "Waiting For Player");
}

void SpawnBot() {
	if (IsInGame() && IsServerHost()) {
		SV_AddTestClient();
		int botNum = GetBotClientNum();
		SetTimeout(TeamSelect, botNum, 200);
		SetTimeout(ClassSelect, botNum, 400);
	}
}

void PreventForfeit() {
	if (IsInGame() && IsServerHost() && Ascension.Variables.preventEnemyForfeit) {
		if (PreventEnemyForfeit())
			Globals.spawnBots = true;
	}
}

bool PreventEnemyForfeit() {
	gclient_s *client;
	for (int i = 0; i < 18; i++) {
		client = &g_clients[i];
		if (SV_IsClientActive(i) && ((client->sess.cs.team != Globals.hostTeam) || client->sess.cs.team == TEAM_FREE))
			return false;
	}
	return true;
}

void Scr_SetNumParam(int num) {
  *(int *)(0x1B9A580 + 0x2038) = num;
}

void KickClient() {
	if (IsInGame() && IsServerHost()) {
		char buff[MAX_STRING_CHARS];
		int clientNum = Ascension.currentClient;
		if (SV_IsClientActive(clientNum)) {
			Ascension.GoBack();
			sprintf(buff, "clientkick %i", clientNum);
			CBuf_AddText(0, buff);
		}
	}
}

void KickClient(int clientNum, const char *reasonFormat, ...) {
	va_list argptr;
	char text[MAX_STRING_CHARS];
	va_start(argptr, reasonFormat);
	_vsnprintf(text, MAX_STRING_CHARS, reasonFormat, argptr);
	va_end(argptr);
	if (SV_IsClientActive(clientNum))
		SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, va("w \"%s\"", text));
}

void UnlockAllOne(int clientNum) {
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "c \"Unlock All: ^2Started\"");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 2056 206426 6525 7F 3760 09 4623 E803 3761 09 4627 F430 3762 02 4631 14 3763 02 4635 3C 3764 02 4639 0F 3765 02 4643 14 3766 02 4647 28 3767 02 4651 0A 3752 09 4591 E803 3753 09 4595 0F40 3754 02 4599 14 3755 02 4603 3C 3756 02 4607 0F 3757 02 4611 14 3758 02 4615 28 3759 02 4619 0A 3736 09 4527 E803");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3737 09 4531 0F40 3738 02 4535 14 3739 02 4539 3C 3740 02 4543 0F 3741 02 4547 14 3742 02 4551 28 3743 02 4555 0A 3799 09 4779 E803 3800 09 4783 0F40 3801 02 4787 14 3802 02 4791 3C 3803 02 4795 0F 3804 02 4799 14 3805 02 4803 28 3806 02 4807 0A");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3775 09 4683 E803 3776 09 4687 0F40 3777 02 4691 14 3778 02 4695 3C 3779 02 4699 0F 3780 02 4703 14 3781 02 4707 28 3782 02 4711 0A 3728 09 4495 E803 3729 09 4499 0F40 3730 02 4503 14 3731 02 4507 3C 3732 02 4511 0F 3733 02 4515 14 3734 02 4519 28 3735 02 4523 0A 3783 09 4715 E803 3784 09 4719 0F40 3785 02 4723 14 3786 02 4727 3C");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3787 02 4731 0F 3788 02 4735 14 3789 02 4739 28 3790 02 4743 0A 3791 09 4747 E803 3864 02 5039 14 3865 02 5043 28 3866 02 5047 09 3888 09 5135 E803 3887 09 5131 0F40");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3792 09 4751 0F40 3793 02 4755 14 3794 02 4759 3C 3795 02 4763 0F 3796 02 4767 14 3797 02 4771 28 3798 02 4775 0A 3744 09 4559 E803 3745 09 4563 0F40 3746 02 4567 14 3889 02 5139 0F 3890 02 5143 3C 3891 02 5147 14 3892 02 5151 28 3893 02 5155 09 3807 09 4811 E803 3808 09 4815 0F40 3809 02 4819 0F 3810 02 4823 14 3811 02 4827 28");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3747 02 4571 3C 3748 02 4575 0F 3749 02 4579 14 3750 02 4583 28 3751 02 4587 0A 3853 09 4995 E803 3854 09 4999 0F40 3855 02 5003 1E 3856 02 5007 3C 3857 02 5011 14 3858 02 5015 28 3859 02 5019 09 3839 09 4939 E803 3840 09 4943 0F40 3841 02 4947 1E 3842 02 4951 3C 3843 02 4955 14 3844 02 4959 28 3845 02 4963 09 3825 09 4883 E803");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3826 09 4887 0F40 3827 02 4891 1E 3828 02 4895 3C 3829 02 4899 14 3830 02 4903 28 3831 02 4907 09 3832 09 4911 E803 3833 09 4915 0F40 3834 02 4919 1E 3835 02 4923 3C 3836 02 4927 14 3837 02 4931 28 3838 02 4935 09 3846 09 4967 E803 3847 09 4971 0F40");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3848 02 4975 1E 3849 02 4979 3C 3850 02 4983 14 3851 02 4987 28 3852 02 4991 09 3768 09 4655 E803 3769 09 4659 0F40 3771 02 4667 0F 3770 02 4663 3C 3772 02 4671 14 3773 02 4675 28 3774 02 4679 09 3874 09 5079 E803 3875 09 5083 0F40 3876 02 5087 0F");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3877 02 5091 3C 3878 02 5095 14 3879 02 5099 28 3880 02 5103 09 3867 09 5051 E803 3868 09 5055 0F40 3869 02 5059 0F 3870 02 5063 3C 3871 02 5067 14 3872 02 5071 28 3873 02 5075 09 3860 09 5023 E803 3861 09 5027 0F40 3862 02 5031 0F 3863 02 5035 3C");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3812 02 4831 06 3813 09 4835 E803 3814 09 4839 0F40 3815 02 4843 0F 3816 02 4847 14 3817 02 4851 28 3818 02 4855 06 3819 09 4859 E803 3820 09 4863 0F40 3821 02 4867 0F 3822 02 4871 14 3823 02 4875 28 3824 02 4879 06 3881 09 5107 E803 3882 09 5111 0F40");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3883 02 5115 0F 3884 02 5119 14 3885 02 5123 28 3886 02 5127 06 3898 09 5175 E803 3899 09 5179 0F40 3894 09 5159 E803 3895 09 5163 0F40 3900 09 5183 E803 3901 09 5187 0F40 3896 09 5167 E803 3897 09 5171 0F40 3902 09 5191 E803 3903 09 5195 0F40 3908 09 5215 E803");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "c \"Unlock All: ^320^7/100\"");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3909 09 5219 0F40 3904 09 5199 E803 3905 09 5203 0F40 3906 09 5207 E803 3907 09 5211 0F40 3912 06 5231 C409 3913 09 5235 0F40 3910 06 5223 C409 3911 09 5227 0F40 3916 09 5247 E803 3917 09 5251 0F40 3914 09 5239 E803 3915 09 5243 0F40 3920 07 5263 C409 3921 09 5267 0F40");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3918 07 5255 C409 3919 09 5259 0F40 3922 09 5271 B004 3923 09 5275 B004 3924 09 5279 B004 3925 09 5283 B004 3926 09 5287 FA 3643 0A 4155 09 3927 07 5292 6108 3931 07 5307 EE02 3938 07 5335 0F40 3932 07 5311 8403 3935 07 5323 EE02 3933 07 5315 E803 3941 07 5347 402414");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3934 07 5319 FA 3936 07 5327 FA 3942 07 5351 0F40 3939 07 5339 64 3928 07 5295 0F40 3930 07 5303 FA 3929 07 5299 FA 3940 07 5343 EE02 3937 07 5331 64 3943 04 5355 32 3944 04 5359 32 3945 04 5363 32 3946 04 5367 32 3947 04 5371 32 3948 04 5375 32");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3949 04 5379 32 3950 04 5383 32 3951 04 5387 19 3952 04 5391 19 3953 04 5395 19 3954 04 5399 19 3955 04 5403 19 3956 04 5407 0A 3957 04 5411 0A 3958 04 5415 E803 3959 04 5419 E803 3960 04 5423 E803 3961 04 5427 E803 3962 04 5431 32 3963 04 5435 1E 3964 04 5439 32 3965 04 5443 1E 3966 04 5447 32 3967 04 5451 1E 3968 04 5455 1E");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3969 02 5459 FF 3972 02 5471 FF 3973 02 5475 FF 3983 02 5515 FF 3984 02 5519 FF 3985 02 5523 FF 3986 02 5527 FF 3987 02 5531 FF 3988 02 5535 FF 4100 02 5983 FF 3970 02 5463 19 3971 02 5467 19 4020 04 5663 1E 4021 04 5667 1E 4022 04 5671 1E 4023 04 5675 0F 4024 04 5679 0F 4025 04 5683 0F");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3989 02 5539 FF 3990 02 5543 FF 3991 02 5547 FF 3992 02 5551 FF 3994 02 5559 FF 3995 02 5563 FF 3996 02 5567 FF 3997 02 5571 FF 4001 02 5587 FF 4002 02 5591 FF 4028 04 5695 50C3 4029 04 5699 50C3 4030 04 5703 64 4035 04 5723 32 4036 04 5727 32 4037 04 5731 32 4038 04 5735 32 4039 04 5739 32 4040 04 5743 32");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 4003 02 5595 FF 4004 02 5599 FF 4005 02 5603 FF 4006 02 5607 FF 4007 02 5611 FF 4008 02 5615 FF 4009 02 5619 FF 4010 02 5623 FF 4011 02 5627 FF 4012 02 5631 FF 4101 04 5987 C8 4103 04 5995 0A 4104 04 5999 1E 4105 04 6003 1E 3993 04 5555 14 3998 04 5575 C8 3999 03 5579 0A 4000 03 5583 0A 4107 04 6011 0F");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 4013 02 5635 FF 4014 02 5639 FF 4015 02 5643 FF 4016 02 5647 FF 4017 02 5651 FF 4018 02 5655 FF 4114 02 6039 FF 4110 02 6023 FF 4106 02 6007 FF 4019 02 5659 FF 4041 04 5747 32 4050 03 5783 19 4051 03 5787 19 4055 03 5803 19 4056 03 5807 19 4065 04 5843 14 4068 04 5855 14 4069 04 5859 14 4058 03 5815 19");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 4026 02 5687 FF 4027 02 5691 FF 4042 02 5751 FF 4031 02 5707 FF 4032 02 5711 FF 4033 02 5715 FF 4034 02 5719 FF 4043 02 5755 FF 4044 02 5759 FF 4045 02 5763 FF 4108 04 6015 32 4109 02 6019 0A 4111 03 6027 0A 4112 03 6031 0A 4113 03 6035 0A 4115 03 6043 0A 4116 05 6047 FA 4117 05 6051 64 4118 05 6055 E803");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 4046 02 5767 FF 4047 02 5771 FF 4048 02 5775 FF 4049 02 5779 FF 4052 02 5791 FF 4053 02 5795 FF 4054 02 5799 FF 4102 02 5991 FF 4121 02 6067 FF 4057 02 5811 FF 4119 05 6059 2C00 4120 05 6063 2C00 6525 7F");
}

void UnlockAllTwo(int clientNum) {
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "c \"Unlock All: ^340^7/100\"");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 4059 02 5819 OO 4060 02 5823 OO 4061 02 5827 OO 4062 02 5831 OO 4063 02 5835 OO 4064 02 5839 OO 4066 02 5847 OO 4067 02 5851 OO 4070 02 5863 OO 4071 02 5867 OO 4072 02 5871 OO 4073 02 5875 OO 4074 02 5879 OO 4075 02 5883 OO 4076 02 5887 OO 4077 02 5891 OO 4078 02 5895 OO 4079 02 5899 OO 4080 02 5903 OO 4081 02 5907 OO");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 4082 02 5911 OO 4083 02 5915 OO 4084 02 5919 OO 4085 02 5923 OO 4086 02 5927 OO 4087 02 5931 OO 4088 02 5935 OO 4089 02 5939 OO 4090 02 5943 OO 4091 02 5947 OO 4092 02 5951 OO 4093 02 5955 OO 4094 02 5959 OO 4095 02 5963 OO 4096 02 5967 OO 4097 02 5971 OO 4098 02 5975 OO 4099 02 5979 OO 4100 02 5983 OO 4099 02 5979 OO");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3038 05 6695 80 6696 10 6697 02 6697 42 6696 11 6696 31 6697 46 6697 C6 6696 33 6696 73 6697 CE 6698 09 6696 7B 6697 CF 6697 EF 6698 0D 6696 7F 6696 FF 6697 FF 6698 0F 6637 84 6637 8C 6503 03 6637 9C 6637 BC 6503 07 6637 FC 6638 FF 6503 0F 6638 03 6638 07");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 6503 1F 6638 0F 6638 1F 6638 3F 6503 3F 6638 7F 6638 FF 6503 7F 6639 FF 6639 03 6639 07 6503 FF 6639 0F 6639 1F 6504 FF 6639 3F 6639 7F 6639 FF 6504 03 6640 09 6640 0B 6504 07 6640 0F 6640 1F 6504 0F 6640 3F 6640 7F 6504 1F 6640 FF 6641 23 6504 3F 6641 27");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3038 05 3550 05 3614 05 3486 05 3422 05 3358 05 3294 05 3230 05 3166 05 3102 05 3038 05 2072 2D302E302F30O 2092 30303130 2128 3130 2136 3B05ZZ3C05 2152 3D05O");

}

void UnlockAllThree(int clientNum) {
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "c \"Unlock All: ^360^7/100\"");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 6641 2F 6504 7F 6641 3F 6641 7F 6504 FF 6641 FF 6642 85 6505 FF 6642 87 6642 8F 6505 03 6642 9F 6642 BF 6505 07 6642 FF 6643 11 6505 0F 6643 13 6643 17 6505 1F 6643 1F 6643 3F 6505 3F 6643 7F 6643 FF 6505 7F 6644 43 6644 47 6505 FF 6644 4F 6644 5F 6506 FF");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 6644 7F 6644 FF 6506 03 6645 09 6645 0B 6506 07 6645 0F 6645 1F 6506 0F 6645 3F 6645 7F 6506 1F 6645 FF 6646 23 6506 3F 6646 27 6646 2F 6506 7F 6646 3F 6646 7F 6506 FF 6646 FF 6647 85 6507 FF 6647 87 6647 8F 6507 03 6647 9F 6647 BF 6507 07 6647 FF 6648 11");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 6507 0F 6648 13 6648 17 6507 1F 6648 1F 6648 3F 6507 3F 6648 7F 6648 FF 6507 7F 6649 FF 6649 03 6649 07 6507 FF 6649 0F 6649 1F 6508 FF 6649 3F 6649 7F 6649 FF 6508 03 6650 FF 6650 03 6508 07 6650 07 6650 0F 6650 1F 6508 0F 6650 3F 6650 7F 6508 1F 6650 FF");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 6651 FF 6651 03 6508 3F 6651 07 6651 0F 6508 7F 6651 1F 6651 3F 6508 FF 6651 7F 6651 FF 6509 FF 6652 FF 6652 03 6509 03 6652 07 6652 0F 6509 07 6652 1F 6652 3F 6509 0F 6652 7F 6652 FF 6509 1F 6653 FF 6653 03 6509 3F 6653 07 6653 0F 6509 7F 6653 1F 6653 3F");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 6509 FF 6653 7F 6653 FF 6510 FF 6654 FF 6654 03 6510 03 6654 07 6654 0F 6510 07 6654 1F 6654 3F 6510 0F 6654 7F 6654 FF 6510 1F 6655 FF 6655 03 6510 3F 6655 07 6655 0F 6510 7F 6655 1F 6655 3F 6510 FF 6655 7F 6655 FF 6511 FF 6656 FF 6656 03 6511 03 6656 07");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 6656 0F 6511 07 6656 1F 6656 3F 6511 0F 6656 7F 6656 FF 6511 1F 6657 FF 6657 03 6511 3F 6657 07 6657 0F 6511 7F 6657 1F 6657 3F 6511 FF 6657 7F 6657 FF 6512 FF 6658 FF 6658 03 6512 03 6658 07 6658 0F 6512 07 6658 1F 6658 9F 6658 BF 6658 FF 6680 FF 6661 5B");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 6661 5F 6661 7F 6661 FF 6673 08 6673 18 6673 38 6673 78 6673 F8 6674 FF 6674 03 6674 07 6674 0F 6674 1F 6674 3F 6674 7F 6674 FF 6679 08 6673 F9 6673 FB 6673 FF 6675 FF 6677 FF 6677 03 6677 07 6677 0F 6677 1F 6677 3F 6677 7F 6677 FF 6679 09 6679 0B 6679 0F");

}

void UnlockAllFour(int clientNum) {
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "c \"Unlock All: ^380^7/100\"");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 6679 1F 6679 3F 6679 7F 6679 FF 6680 03 6680 07 6680 0F 6680 1F 6680 3F 6680 BF 6681 FF 6681 03 6681 0B 6681 1B 6681 3B 6681 7B 6681 FB 6681 FF 6680 FF 6686 FF 6632 FF 6632 03 6632 07 6632 0F 6632 1F 6632 3F 6632 7F 6632 FF 6633 FF 6633 03 6633 07 6633 0F");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 6633 1F 6633 3F 6633 7F 6633 FF 6634 FF 6634 03 6634 07 6634 0F 6634 1F 6634 3F 6634 7F 6634 FF 6635 FF 6635 03 6635 07 6635 0F 6635 1F 6635 3F 6635 7F 6635 FF 6636 FF 6636 03 6636 07 6636 0F 6636 1F 6636 3F 6636 7F 6636 FF 6637 FD 6637 FF 6690 FF 6690 03");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 6690 07 6690 0F 6690 1F 6690 3F 6690 7F 6690 FF 6695 81 6695 83 6695 87 6695 8F 6695 9F 6695 BF 6698 1F 6698 3F 6698 7F 6698 FF 6699 C1 6699 C3 6699 C7 6699 CF 6699 DF 6699 FF 6700 1F 6700 3F 6700 7F 6700 FF 6701 03 6701 07 6701 0F 6701 1F 6701 3F 6701 7F");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 6701 FF 6702 FF 6702 03 6702 07 6524 10 6524 30 6524 70 6524 F0 6529 FF 6529 03 6529 07 6530 08 6529 0F 6529 1F 6529 3F 6529 7F 6529 FF 6530 09 6530 0B 6530 0F 6530 1F 6530 7F 6530 FF 6531 FF 6531 03 6531 07 6531 0F 6531 1F 6531 3F 6531 7F 6531 FF 6532 FF");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 6532 03 6532 07 6532 0F 6512 C7 6526 02 6512 D7 6526 06 6512 F7 6526 86 6532 1F 6532 3F 6532 BF 6533 F9 6533 FB 6533 FF 6532 FF 6526 87 6526 A7 6512 FF 6540 7F 6526 E7 6526 EF 6526 FF 6517 FF 6527 FF 6528 FF 6522 FF 6524 F1 6524 F3 6524 F7 6524 FF");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3850 99 3851 99 3852 99 3853 99 3854 99 3855 99 3856 99 3857 99 3858 99 3859 99 3860 99 3861 99 3862 99 3863 99 3864 99 3865 99 3866 99 3867 99 3868 99 3869 99 3870 99 3871 99 3872 99 3873 99 3874 99 3875 99 3876 99 3877 99 3878 99 3879 99 3880 99 3881 99 3882 99 3883 99 3884 99 3885 99 3886 99 3887 99 3888 99 3889 99 3890 99 3891 99 3892 99 3893 99 3894 99 3895 99 3896 99 3897 99 3898 99 3899 99 3900 99");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3900 99 3901 99 3902 99 3903 99 3904 99 3905 99 3906 99 3907 99 3908 99 3909 99 3910 99 3911 99 3912 99 3913 99 3914 99 3915 99 3916 99 3917 99 3918 99 3919 99 3920 99 3921 99 3922 99 3923 99 3924 99 3925 99 3926 99 3927 99 3928 99 3929 99 3930 99 3931 99 3932 99 3933 99 3934 99 3935 99 3936 99 3937 99 3938 99 3939 99 3940 99 3941 99 3942 99 3943 99 3944 99 3945 99 3946 99 3947 99 3948 99 3949 99 3950 99");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 3950 99 3951 99 3952 99 3953 99 3954 99 3955 99 3956 99 3957 99 3958 99 3959 99 3960 99 3961 99 3962 99 3963 99 3964 99 3965 99 3966 99 3967 99 3968 99 3969 99 3970 99 3971 99 3972 99 3973 99 3974 99 3975 99 3976 99 3977 99 3978 99 3979 99 3980 99 3981 99 3982 99 3983 99 3984 99 3985 99 3986 99 3987 99 3988 99 3989 99 3990 99 3991 99 3992 99 3993 99 3994 99 3995 99 3996 99 3997 99 3998 99 3999 99 4000 99");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 4000 99 4001 99 4002 99 4003 99 4004 99 4005 99 4006 99 4007 99 4008 99 4009 99 4010 99 4011 99 4012 99 4013 99 4014 99 4015 99 4016 99 4017 99 4018 99 4019 99 4020 99 4021 99 4022 99 4023 99 4024 99 4025 99 4026 99 4027 99 4028 99 4029 99 4030 99 4031 99 4032 99 4033 99 4034 99 4035 99 4036 99 4037 99 4038 99 4039 99 4040 99 4041 99 4042 99 4043 99 4044 99 4045 99 4046 99 4047 99 4048 99 4049 99 4050 99");

}

void UnlockAllFive(int clientNum) {
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "N 4050 99 4051 99 4052 99 4053 99 4054 99 4055 99 4056 99 4057 99 4058 99 4059 99 4060 99 4061 99 4062 99 4063 99 4064 99 4065 99 4066 99 4067 99 4068 99 4069 99 4070 99 4071 99 4072 99 4073 99 4074 99 4075 99 4076 99 4077 99 4078 99 4079 99 4080 99 4081 99 4082 99 4083 99 4084 99 4085 99 4086 99 4087 99 4088 99 4089 99 4090 99 4091 99 4092 99 4093 99 4094 99 4095 99 4096 99 4097 99 4098 99 4099 99 4100 99");
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, "c \"Unlock All: ^2Complete\"");
}

void UnlockAllClient() {
	if (IsInGame() && IsServerHost()) {
		int clientNum = Ascension.currentClient;
		SetTimeout(UnlockAllOne, clientNum, 0);
		SetTimeout(UnlockAllTwo, clientNum, 500);
		SetTimeout(UnlockAllThree, clientNum, 1000);
		SetTimeout(UnlockAllFour, clientNum, 1500);
		SetTimeout(UnlockAllFive, clientNum, 2000);
	}
	
}

void FastRestart() {
	if (IsInGame() && IsServerHost()) {
		Ascension.Close();
		CBuf_AddText(0, "fast_restart");
	}
}

void GiveWeapon(int clientNum, const char *weaponName, int camo, bool akimbo) {
	int index = G_GetWeaponIndexForName(weaponName);
	gclient_s *client = &g_clients[clientNum];
	gentity_s *pEnt = &g_entities[clientNum];
	G_GivePlayerWeapon(client, index, camo, akimbo);
	G_InitializeAmmo(pEnt, index, akimbo, camo);
}

void DropItem(int clientNum, const char *weaponName) {
	gentity_s *pEnt = &g_entities[clientNum];
	playerState_s *ps = &g_clients[clientNum].ps;

	int index = G_GetWeaponIndexForName(weaponName);
	int model = BG_GetWeaponModel(ps, index);

	if (*(int *)(BG_GetWeaponDef(index) + 0x38) == 3)
		return;

	int weaponEnt = Drop_Weapon(pEnt, index, model, SL_GetString("j_gun", 0));
	if (weaponEnt) {
		//Item_SetStateFromTag(pEnt, SL_GetString("j_gun", 0), weaponEnt);
		Item_SetDefaultVelocity(pEnt, weaponEnt);
		G_EntEnablePhysics(pEnt, *(int *)(BG_GetWeaponDef(index) + 0x3C8));
	}
}

void DropCanswap() {
	int clientNum = GetSelfClientNum();
	const char *weaponName = "aa12_fmj_mp";
	GiveWeapon(clientNum, weaponName, 0, false);
	DropItem(clientNum, weaponName);
}

int GetLevelTime() {
	return *(int *)(0x1316880 + 0x3AC);
}

int GetBotClientNum() {
	for (int i = 0; i < 18; i++) {
		if (SV_IsTestClient(i))
			return i;
	}
	return -1;
}

void MoveBot() {
	if (IsInGame() && IsServerHost()) {
		int clientNum = GetSelfClientNum();
		gclient_s *self = &g_clients[clientNum];
		int botNum = GetBotClientNum();
		gclient_s *bot = &g_clients[botNum];
		bot->ps.SetOrigin(self->ps.origin);
		bot->ps.SetPlayerAngles(self->ps.viewAngles);
	}
}

void SetUAV(int clientNum, bool hasRadar) {
	bool ffa = Globals.hostTeam == TEAM_FREE;
	int radar = hasRadar ? 0x1 : 0x0;
	if (ffa || g_clients[clientNum].sess.cs.team == Globals.hostTeam)
		memset(&g_clients[clientNum].sess.hasRadar, radar, sizeof(int));
}

void ConstantUAV() {
	for (int i = 0; i < 18; i++) 
		SetUAV(i, Ascension.Variables.sweepingUAV);
}

void ChangeBackground() {
	Inject(GetMaterial("mw2_main_sp_image"), ASSETS_PATH"SpaceMW2.dds");
}

void Inject(Material *material, const char *imagePath) {

}

int SV_Cmd_Argc() {
	return sv_cmd_args->argc[sv_cmd_args->nesting];
}

const char * SV_Cmd_Argv(int argIndex) {
	if (argIndex >= sv_cmd_args->argc[sv_cmd_args->nesting])
		return "";
	else
		return sv_cmd_args->argv[sv_cmd_args->nesting][argIndex];
}

int MSG_GetByte(msg_t *msg, int where) {
	if (where < msg->cursize)
		return msg->data[where];
	else
		return msg->splitData[where - msg->cursize];
}

int MSG_ReadBits(msg_t *msg, int bits) {
	int value = 0;
	for (int i = 0; i < bits; i++) {
		int bit = msg->bit & 7;
		if (!bit) {
			if (msg->readcount >= msg->splitSize + msg->cursize) {
				msg->overflowed = 1;
				return -1;
			}
			msg->bit = 8 * msg->readcount++;
		}
		value |= ((MSG_GetByte(msg, msg->bit >> 3) >> bit) & 1) << i;
		msg->bit++;
	}
	return value;
}

void FreezeControls(int clientNum, bool freeze) {
	int buf = freeze ? 0x04 : 0x00;
	gclient_s *client = &g_clients[clientNum];
	memcpy(&client->mFlags, &buf, sizeof(buf));
}

void iPrintln(int clientNum, const char *messageFormat, ...) {
	va_list argptr;
	char text[MAX_STRING_CHARS];
	va_start(argptr, messageFormat);
	_vsnprintf(text, MAX_STRING_CHARS, messageFormat, argptr);
	va_end(argptr);
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, va("f \"%s\"", text));
}

void iPrintlnBold(int clientNum, const char *messageFormat, ...) {
	va_list argptr;
	char text[MAX_STRING_CHARS];
	va_start(argptr, messageFormat);
	_vsnprintf(text, MAX_STRING_CHARS, messageFormat, argptr);
	va_end(argptr);
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, va("c \"%s\"", text));
}