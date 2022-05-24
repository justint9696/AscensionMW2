#include "Handle.h"

handle_s events[HANDLE_COUNT];

unsigned int GetCurrentTick() {
	return *(int *)(0x1316880 + 0x3AC);
}

void ClearEvents() {
	memset(&events, 0, sizeof(events));
}

handle_t SetTimeout(void(*function)(int), int arg, clock_t startMs) {
	if (function == NULL)
		return INVALID_HANDLE;

	handle_s *ev;
	for (int32_t i = 1; i < HANDLE_COUNT; i++) {
		ev = &events[i];
		if (ev->function == NULL) {
			ev->function = function;
			ev->arg = arg;
			ev->type = CALL_TEMP;
			ev->handle = NULL;
			ev->interval = 0;
			ev->startTime = GetCurrentTick() + startMs;
			ev->time = ev->startTime;
			return (handle_t)i;
		}
	}
	return INVALID_HANDLE;
}

void ExecuteEvents() {
	clock_t time;
	void(*function)(int);
	handleCall_t type;
	handle_s *ev;

	for (handle_t i = 0; i < HANDLE_COUNT; i++) {
		ev = &events[i];
		if ((function = ev->function) != NULL) {
			if (ev->type != CALL_FREE) {
				time = GetCurrentTick();
				if (ev->startTime <= time) {
					if (ev->time <= time) {
						ev->time += ev->interval;
						if (ev->handle == NULL || ev->time > time)
							function(ev->arg);
						switch (ev->type) {
						default:
							break;
						case CALL_START:
							ev->type = CALL_EXECUTE;
							break;
						case CALL_TEMP:
						case CALL_STOP:
							memset(ev, 0, sizeof(handle_s));
							break;
						}
					}
				}
			}
		}
	}
}
