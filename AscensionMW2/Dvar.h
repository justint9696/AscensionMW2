#ifndef _DVAR_
#define _DVAR_

enum DvarType {
	Dvar_Bool = 0x00,
	Dvar_Float = 0x01,
	Dvar_Vec2 = 0x02,
	Dvar_Vec3 = 0x03,
	Dvar_Vec4 = 0x04,
	Dvar_Int = 0x05,
	Dvar_Enum = 0x06,
	Dvar_String = 0x07,
	Dvar_Color = 0x08,
	Dvar_DevTweak = 0x09
};

enum DvarSetSource {
	DVAR_SOURCE_INTERNAL = 0x0,
	DVAR_SOURCE_EXTERNAL = 0x1,
	DVAR_SOURCE_SCRIPT = 0x2
};

union DvarLimits {
	struct enumeration {
		int stringCount;
		char **strings;
	};

	struct integer {
		int min;
		int max;
	};

	struct decimal {
		float min;
		float max;
	};
};

union DvarValue {
	bool enabled;
	int integer;
	unsigned int unsignedInt;
	float value;
	float vec[4];
	char *string;
	char color[4];
};

struct dvar_s {
	char *dvarName;
	short flags;
	DvarType type;
	bool modified;
	DvarValue current;
	DvarValue latched;
	DvarValue reset;
	DvarLimits domain;
	dvar_s * next;
};

void SetDvar(const char *dvarName, const char *value);
void ResetDvar(const char *dvarName);
void SetClientDvar(int clientNum, const char *dvarName, const char *value);

typedef dvar_s *(__cdecl *Dvar_FindVar_t)(const char *dvarName);
extern Dvar_FindVar_t Dvar_FindVar;

typedef void(__cdecl *Dvar_Reset_t)(dvar_s *dvar, DvarSetSource setSource);
extern Dvar_Reset_t Dvar_Reset;

typedef void(__cdecl *Dvar_SetFromStringByName_t)(const char *dvarName, const char *value);
extern Dvar_SetFromStringByName_t Dvar_SetFromStringByName;

typedef bool(__cdecl *Dvar_GetBool_t)(const char *dvarName);
extern Dvar_GetBool_t Dvar_GetBool;

typedef bool(__cdecl *Dvar_GetInt_t)(const char *dvarName);
extern Dvar_GetBool_t Dvar_GetInt;

#endif