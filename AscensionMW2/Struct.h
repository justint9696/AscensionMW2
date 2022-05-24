#ifndef _STRUCT_
#define _STRUCT_

#include <sys/types.h>
#include <stdarg.h>

typedef void Font_s;
typedef void Material;
typedef void PartyData_s;
typedef void UiContext;
typedef void SessionData;
typedef void VariableValue;

#define FONT_SMALL_DEV		"fonts/smallDevFont"
#define FONT_BIG_DEV		"fonts/bigDevFont"
#define FONT_CONSOLE		"fonts/consoleFont"
#define FONT_BIG			"fonts/bigFont"
#define FONT_SMALL			"fonts/smallFont"
#define FONT_BOLD			"fonts/boldFont"
#define FONT_NORMAL			"fonts/normalFont"
#define FONT_OBJECTIVE		"fonts/objectiveFont"
#define FX_MATERIAL			"fxMaterial"
#define FX_GLOW_MATERIAL	"fxGlowMaterial"

#define MAX_NETNAME 32
#define MAX_NETCLANNAME 4
#define MAX_CLIENTS 18
#define MAX_STRING_CHARS 1024

#define CMD_MAX_NESTING 8

#define ASSETS_PATH "/dev_hdd0/tmp/Ascension/"

#define cg (reinterpret_cast<cg_s *>(*(int *)0x915254))
#define contentPack (reinterpret_cast<ContentPackList *>(0x1F96708))
#define stats (reinterpret_cast<stats_s *>(0x1FF9A94))
#define xenonUserData (reinterpret_cast<XenonUserData *>(0x1F9F118))
#define g_serverSession (reinterpret_cast<SessionData *>(0x1FF49B0))
#define g_entities (reinterpret_cast<gentity_s *>(0x1319800))
#define g_clients (reinterpret_cast<gclient_s *>(0x14E2200))
#define g_lobbyData (reinterpret_cast<PartyData_s *>(0xA13558))
#define svs (reinterpret_cast<serverStatic_t *>(0x1BF5100))
#define ucmds (reinterpret_cast<ucmd_t *>(0x8AA288))
#define sv_cmd_args (reinterpret_cast<CmdArgs *>(0x17476F4))

struct scr_entref_t {
	unsigned short entnum;
	unsigned short classnum;
};

typedef struct vec3_t {
	float x, y, z;
} vec3;

typedef struct vec2_t {
	float x, y;
} vec2;

enum clc_ops_e {
	clc_move = 0x0,
	clc_moveNoDelta = 0x1,
	clc_clientCommand = 0x2,
	clc_EOF = 0x3,
};

struct msg_t {
	int overflowed;
	int readOnly;
	char *data;
	char *splitData;
	int maxsize;
	int cursize;
	int splitSize;
	int readcount;
	int bit;
	int lastEntityRef;
};

enum svscmd_type {
	SV_CMD_CAN_IGNORE = 0x0,
	SV_CMD_RELIABLE = 0x1,
};

enum clientConnected_t {
	CON_DISCONNECTED = 0x0,
	CON_CONNECTING = 0x1,
	CON_CONNECTED = 0x2,
};

enum sessionState_t {
	SESS_STATE_PLAYING = 0x0,
	SESS_STATE_DEAD = 0x1,
	SESS_STATE_SPECTATOR = 0x2,
	SESS_STATE_INTERMISSION = 0x3,
};

enum clientStateStatus_t {
	CS_FREE = 0x0,
	CS_ZOMBIE = 0x1,
	CS_RECONNECTING = 0x2,
	CS_CONNECTED = 0x3,
	CS_CLIENTLOADING = 0x4,
	CS_ACTIVE = 0x5,
};

struct usercmd_s {
	int serverTime; // 0x0
	int buttons; // 0x4
	int angles[3]; // 0x8
	char forwardmove; // 0x14
	char rightmove; // 0x15
	char undefined2[2]; // 0x16
	float meleeChargeYaw; // 0x18
	char meleeChargeDist; // 0x1C
	char unk[4]; // 0x1D
	char selectedLoc[2]; // 0x21
	char selectedLocAngle; // 0x3
	int unknown; // 0x24
};

struct playerTeamState_t {
	int location; // 0x0
};

struct viewClamp {
	vec2 start; // 0x0
	vec2 current; // 0x8
	vec2 goal; // 0x10
};

struct viewClampState {
	viewClamp min; // 0x0
	viewClamp max; // 0x18
	float accelTime; // 0x30
	float decelTime; // 0x34
	float totalTime; // 0x38
	float startTime; // 0x3C
};

enum team_t {
	TEAM_FREE = 0x0,
	TEAM_AXIS = 0x1,
	TEAM_ALLIES = 0x2,
	TEAM_SPECTATOR = 0x3,
};

struct clientState_s {
	int clientNum;
	team_t team;
	char unk0[0x3C];
	char name[32];
	char unk1[0x4];
	int rank;
	int prestige;
	char unk2[0x10];
	char clanName[4];
	char unk3[0x8];
};

struct clientSession_s {
	sessionState_t sessionState;
	char unk0[0xB8];
	clientState_s cs;
	char unk2[0xB];
	int hasRadar;
	int isRadarBlocked;
	char unk3[0x130];
};

struct playerState_s {
	char unk0[0x1C];
	float origin[3];
	float velocity[3];
	char unk1[0x2C];
	float deltaAngles[3];
	char unk2[0xA0];
	float viewAngles[3];
	char unk3[0x3078];

	void SetOrigin(float *origin);
	void SetPlayerAngles(float *viewAngles);
	void SetVelocity(float *velocity);
};

struct gclient_s {
	playerState_s ps; // 0x0
	clientSession_s sess; // 0x3190
	int spectatorClient; // 0x341C
	int mFlags; // 0x3420
	char unk0[0x2DC]; // 0x3424
};

struct gentity_s {
	char unk0[0x158];
	gclient_s *client;
	char unk1[0x124];
};

typedef struct ScreenPlacement_t {
	float scaleVirtualToReal[2];
	float scaleVirtualToFull[2];
	float scaleRealToVirtual[2];
	float realViewportPosition[2];
	float realViewportSize[2];
	float virtualViewableMin[2];
	float virtualViewableMax[2];
	float realViewableMin[2];
	float realViewableMax[2];
	float virtualAdjustableMin[2];
	float virtualAdjustableMax[2];
	float realAdjustableMin[2];
	float realAdjustableMax[2];
	float subScreenLeft;
} ScreenPlacement;

struct XenonUserData {
	int signinState;
	char gamertag[MAX_NETNAME];
	uint64_t xuid;
	char xuidString[17];
};

struct stats_s {
	uint32_t xp; // 0x00
	char unk0[4]; // 0x04
	uint8_t prestige; // 0x08
	char unk1[7];	// 0x09
	uint32_t score;	// 0x10
	uint32_t kills;	// 0x14
	uint32_t streaks; // 0x18
	uint32_t deaths; // 0x1C
	char unk2[4]; // 0x20
	uint32_t assists; // 0x24
	uint32_t headshots;	 // 0x28
	char unk3[8]; // 0x2C
	uint64_t timePlayed; // 0x34
	char unk4[8]; // 0x3C
	uint32_t wins; // 0x48
	uint32_t losses; // 0x4C
	uint32_t ties; // 0x50
	uint32_t winStreak; // 0x54
};

enum testClientType_t {
	TC_NONE = 0x0,
	TC_TEST_CLIENT = 0x1,
	TC_BOT = 0x2,
	TC_COUNT = 0x3,
};

struct client_s {
	clientStateStatus_t state;
	char unk0[0x20E90];
	int lastClientCommand;
	char lastClientCommandString[1024];
	char unk1[0x4];
	char name[32];
	char unk2[0x10844];
	testClientType_t testClient;
	int serverId;
	char unk3[0x66478];
};

struct serverStatic_t {
	char unk0[0x4018];
	int clientCount;
	client_s *clients;
};

struct ucmd_t {
	char *name;
	void(*func)(client_s *);
	int allowFromOldServer;
};

struct CmdArgs {
	int nesting;
	int localClientNum[8];
	int controllerIndex[8];
	int argc[CMD_MAX_NESTING];
	const char **argv[CMD_MAX_NESTING];
};

struct ContentPackList {
	int contentPackage;
	char unk0[0x88];
	char dlcName1[0x5];
	char unk1[0x83];
	char dlcName2[0x5];
};

struct clientInfo_s {
	char unk0[0xC];
	char name[32];
	char unk1[0x518];
};

struct cg_s {
	char unk0[0xF6E80];
	clientInfo_s info[18];
};

#endif
