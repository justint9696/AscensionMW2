#ifndef _PS3_
#define _PS3_

void Sleep(usecond_t ms);
uint32_t random_number();
int32_t sys_ss_random_number_generator(void *buf, size_t size);
int32_t sys_dbg_read_process_memory(uint64_t address, void *data, size_t size);

template<typename T>
int32_t ReadProcessMemory(uint32_t address, T data, size_t size);

int32_t sys_dbg_write_process_memory(uint64_t address, const void *data, size_t size);

template<typename T>
int32_t WriteProcessMemory(uint32_t address, const T value, size_t size);

int32_t HookFunction(uint32_t address, uint32_t function);
void HookFunctionStart(uint32_t functionStartAddress, uint32_t newFunction, uint32_t functionStub);
void DrawOkayMessageDialog(const char *format, ...);
void YesNoDialogCallBack(int button_type, void *userdata);
bool DrawYesNoMessageDialog(const char *format, ...);

extern bool yesno_dialog_result;
extern bool yesno_dialog_input;

#endif