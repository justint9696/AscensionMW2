#ifndef _HANDLE_
#define _HANDLE_

#include <string.h>
#include <sys/syscall.h>
#include <cell.h>
#include <sys/types.h>

#define HANDLE_COUNT        0x40
#define INVALID_HANDLE		((handle_t)-1)

typedef uint32_t handle_t;

enum handleCall_t {
	CALL_START = 0x1,
	CALL_EXECUTE = 0x2,
	CALL_TEMP = 0x3,
	CALL_STOP = 0x4,
	CALL_FREE = 0x5,
};

struct handle_s {
	void(*function)(int);
	int arg;
	handle_t* handle;
	handleCall_t type;
	clock_t interval;
	clock_t time;
	clock_t startTime;
};

unsigned int GetCurrentTick();
void ClearEvents();
handle_t SetTimeout(void(*function)(int), int arg, clock_t startMs);
void ExecuteEvents();

#endif