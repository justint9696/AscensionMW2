#include <string.h>
#include "Struct.h"

void playerState_s::SetOrigin(float *origin) {
	memcpy((void *)this->origin, (void *)origin, sizeof(float) * 3);
}

void playerState_s::SetPlayerAngles(float *viewAngles) {
	memcpy((void *)this->viewAngles, (void *)viewAngles, sizeof(float) * 3);
}

void playerState_s::SetVelocity(float *velocity) {
	memcpy((void *)this->velocity, (void *)velocity, sizeof(float) * 3);
}