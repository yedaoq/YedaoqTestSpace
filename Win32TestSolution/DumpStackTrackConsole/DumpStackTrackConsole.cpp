// DumpStackTrackConsole.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include "base/at_exit.h"
#include "breakpad/client/windows/handler/exception_handler.h"
#include "base/process_util.h"
#include <shlwapi.h>

#pragma comment(lib, "base.lib")
#pragma comment(lib, "base_static.lib")
#pragma comment(lib, "dynamic_annotations.lib")
#pragma comment(lib, "exception_handler.lib")
#pragma comment(lib, "breakpad_common.lib")
#pragma comment(lib, "crash_generation_client.lib")
#pragma comment(lib, "shlwapi.lib")

bool MyFilterCallback(void* context, EXCEPTION_POINTERS* exinfo, MDRawAssertionInfo* assertion);
bool MyExecptionCallback(const wchar_t *dump_path, const wchar_t *id, void *context, EXCEPTION_POINTERS *exinfo, MDRawAssertionInfo *assertion,	bool succeeded);
int Error();
int Error1();
int Error2();

int dump_stack(unsigned long ebp, void **stack, int size) {
	int depth = 0;

	/* 2.逐层回溯栈基址 */
	for (depth = 0; (depth < size) && (0 != ebp) && (0 != *(unsigned long *)ebp) && (ebp != *(unsigned long *)ebp); ++depth) {
		stack[depth] = (void *)(*(unsigned long *)(ebp + sizeof(unsigned long)));
		ebp = *(unsigned long *)ebp;
	}

	return depth;
}

int _tmain(int argc, _TCHAR* argv[])
{
	base::AtExitManager at_exit_manager;
	std::locale prev_loc = std::locale::global(std::locale("chs"));

	TCHAR dump_dir[2048];
	GetModuleFileName(NULL, dump_dir, ARRAYSIZE(dump_dir) - 1);
	PathRemoveFileSpec(dump_dir);
	PathAddBackslash(dump_dir);
	google_breakpad::ExceptionHandler eh(dump_dir, MyFilterCallback, MyExecptionCallback, NULL, google_breakpad::ExceptionHandler::HANDLER_ALL);

	Error1();

	return 0;
}

bool MyExecptionCallback( const wchar_t *dump_path, const wchar_t *id, void *context, EXCEPTION_POINTERS *exinfo, MDRawAssertionInfo *assertion, bool succeeded )
{


	void* stack[20] = {0};

	MEMORY_BASIC_INFORMATION MemInfo;

	// VirtualQuery can be used to get the allocation base associated with a
	// code address, which is the same as the ModuleHandle. This can be used
	// to get the filename of the module that the crash happened in.
	if (VirtualQuery((void*)exinfo->ContextRecord->Eip, &MemInfo, sizeof(MemInfo)))
	{
		stack[0] = MemInfo.AllocationBase;
	}

	stack[1] = (void*)Error2;
	stack[2] = (void*)Error1;
	stack[3] = (void*)Error;


	dump_stack(exinfo->ContextRecord->Ebp, stack + 4, 18);

	for (int i = 0; stack[i];++i)
	{
		char buf[20];
		sprintf(buf, "0x%08X\r\n", stack[i]);
		OutputDebugStringA(buf);
	}

	return true;
}

int Error2()
{
	SYSTEMTIME t;
	GetSystemTime(&t);

	return Error1();
}

int Error1()
{
	SYSTEMTIME t;
	GetLocalTime(&t);

	return Error();
}

bool MyFilterCallback( void* context, EXCEPTION_POINTERS* exinfo, MDRawAssertionInfo* assertion )
{
	OutputDebugStringA("test");
	return true;
}

int Error()
{
	//throw 0;
	int a = GetTickCount();
	int b = a % 0xFFFFFFFFFF;
	return 1/(b - a);
}
