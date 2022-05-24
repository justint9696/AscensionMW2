#include <stdarg.h>
#include <string.h>

#include <sys/process.h>
#include <sys/random_number.h>
#include <sys/syscall.h>
#include <sys/timer.h>

#include <sysutil/sysutil_msgdialog.h>
#include <sysutil/sysutil_oskdialog.h>

#include "PS3.h"
#include "Functions.h"

void Sleep(usecond_t ms) {
	sys_timer_usleep(ms * 1000);
}

uint32_t random_number() {
    uint32_t n;
    if (sys_get_random_number(&n, sizeof(uint32_t)) == CELL_OK)
        return n;
    return 0;
}

int32_t sys_ss_random_number_generator(void *buf, size_t size) {
	system_call_3(0x361, (uint64_t)sys_process_getpid(), (uint64_t)buf, size);
	return_to_user_prog(int32_t);
}

int32_t sys_dbg_read_process_memory(uint64_t address, void *data, size_t size) {
	system_call_4(904, (uint64_t)sys_process_getpid(), address, size, (uint64_t)data);
	return_to_user_prog(int32_t);
}

template<typename T>
int32_t ReadProcessMemory(uint32_t address, T data, size_t size) {
	return sys_dbg_read_process_memory(address, &data, size);
}

int32_t sys_dbg_write_process_memory(uint64_t address, const void *data, size_t size) {
	system_call_4(905, (uint64_t)sys_process_getpid(), address, size, (uint64_t)data);
	return_to_user_prog(int32_t);
}

template<typename T>
int32_t WriteProcessMemory(uint32_t address, const T value, size_t size) {
	return sys_dbg_write_process_memory(address, &value, size);
}

template int32_t WriteProcessMemory<uint8_t>(uint32_t address, uint8_t value, size_t size);

template int32_t WriteProcessMemory<bool>(uint32_t address, bool value, size_t size);

template int32_t WriteProcessMemory<uint16_t>(uint32_t address, uint16_t value, size_t size);

template int32_t WriteProcessMemory<uint32_t>(uint32_t address, uint32_t value, size_t size);

int32_t HookFunction(uint32_t address, uint32_t function) {
	uint32_t opcode[4];
	opcode[0] = 0x3D600000 + ((function >> 16) & 0xFFFF);
	opcode[1] = 0x616B0000 + (function & 0xFFFF);
	opcode[2] = 0x7D6903A6;
	opcode[3] = 0x4E800420;
	return sys_dbg_write_process_memory(address, &opcode, 0x10);
}

void HookFunctionStart(uint32_t functionStartAddress, uint32_t newFunction, uint32_t functionStub) {
	uint32_t normalFunctionStub[8], hookFunctionStub[4];
	sys_dbg_read_process_memory(functionStartAddress, normalFunctionStub, 0x10);
	normalFunctionStub[4] = 0x3D600000 + ((functionStartAddress + 0x10 >> 16) & 0xFFFF);
	normalFunctionStub[5] = 0x616B0000 + (functionStartAddress + 0x10 & 0xFFFF);
	normalFunctionStub[6] = 0x7D6903A6;
	normalFunctionStub[7] = 0x4E800420;
	sys_dbg_write_process_memory(functionStub, normalFunctionStub, 0x20);
	hookFunctionStub[0] = 0x3D600000 + ((newFunction >> 16) & 0xFFFF);
	hookFunctionStub[1] = 0x616B0000 + (newFunction & 0xFFFF);
	hookFunctionStub[2] = 0x7D6903A6;
	hookFunctionStub[3] = 0x4E800420;
	sys_dbg_write_process_memory(functionStartAddress, hookFunctionStub, 0x10);
}

void DrawOkayMessageDialog(const char *format, ...) {
	va_list argptr;
	char text[CELL_MSGDIALOG_STRING_SIZE];
	va_start(argptr, format);
	_vsnprintf(text, CELL_MSGDIALOG_STRING_SIZE, format, argptr);
	va_end(argptr);
	cellMsgDialogOpen2(CELL_MSGDIALOG_TYPE_SE_TYPE_NORMAL | CELL_MSGDIALOG_TYPE_BUTTON_TYPE_OK | CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_ON | CELL_MSGDIALOG_TYPE_DEFAULT_CURSOR_OK, text, NULL, NULL, NULL);
}

bool yesno_dialog_result = false;
bool yesno_dialog_input = false;

void YesNoDialogCallBack(int button_type, void *userdata) {
	switch (button_type) {
	case CELL_MSGDIALOG_BUTTON_YES:
		yesno_dialog_result = true;
		break;
	case CELL_MSGDIALOG_BUTTON_NO:
		yesno_dialog_result = false;
		break;
	}
	yesno_dialog_input = false;
}

bool DrawYesNoMessageDialog(const char *format, ...) {
	va_list argptr;
	char text[CELL_MSGDIALOG_STRING_SIZE];
	va_start(argptr, format);
	_vsnprintf(text, CELL_MSGDIALOG_STRING_SIZE, format, argptr);
	va_end(argptr);
	cellMsgDialogOpen2(CELL_MSGDIALOG_TYPE_SE_TYPE_NORMAL | CELL_MSGDIALOG_TYPE_BUTTON_TYPE_YESNO | CELL_MSGDIALOG_TYPE_DISABLE_CANCEL_ON | CELL_MSGDIALOG_TYPE_DEFAULT_CURSOR_NO, text, YesNoDialogCallBack, NULL, NULL);
	yesno_dialog_input = true;
	while (yesno_dialog_input) {
		cellSysutilCheckCallback();
		Sleep(20);
	}
	return yesno_dialog_result;
}
