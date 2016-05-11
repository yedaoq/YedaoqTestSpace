// DumpStackTrackConsole.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include "base/at_exit.h"
#include "breakpad/client/windows/handler/exception_handler.h"
#include "base/process_util.h"
#include <shlwapi.h>
#include <strsafe.h>

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

void PrintLastError(LPCTSTR fun_name);

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
// 	void* stack[30] = {0};
// 	int	  stack_idx = 0;
// 
// 	HANDLE process_handle = GetCurrentProcess();
// 	DuplicateHandle(process_handle, process_handle, process_handle, &process_handle, NULL, FALSE, DUPLICATE_SAME_ACCESS);
// 
// 	STACKFRAME64 stack_frame = {0};
// 	stack_frame.AddrPC.Mode = AddrModeFlat;
// 	stack_frame.AddrPC.Offset = exinfo->ContextRecord->Eip;
// 	stack_frame.AddrStack.Mode = AddrModeFlat;
// 	stack_frame.AddrStack.Offset = exinfo->ContextRecord->Esp;
// 	stack_frame.AddrFrame.Mode = AddrModeFlat;
// 	stack_frame.AddrFrame.Offset = exinfo->ContextRecord->Ebp;
// 
// 	CONTEXT  thread_context_1 = {0};
// 
// 	stack[stack_idx++] = (void*)stack_frame.AddrPC.Offset;
// 
// 	if(!StackWalk64(IMAGE_FILE_MACHINE_I386, process_handle, GetCurrentThread(),  &stack_frame, &thread_context_1, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL ))
// 		PrintLastError(TEXT("StackWalk64"));
// 
// 	stack[stack_idx++] = (void*)stack_frame.AddrPC.Offset;
// 	stack[stack_idx++] = (void*)thread_context_1.Eip;
// 
// 	if(!StackWalk64(IMAGE_FILE_MACHINE_I386, process_handle, GetCurrentThread(),  &stack_frame, &thread_context_1, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL ))
// 		PrintLastError(TEXT("StackWalk641"));
// 
// 	stack[stack_idx++] = (void*)stack_frame.AddrPC.Offset;
// 	stack[stack_idx++] = (void*)thread_context_1.Eip;
// 
// 	if(!StackWalk64(IMAGE_FILE_MACHINE_I386, process_handle, NULL,  &stack_frame, &thread_context_1, NULL, NULL, NULL, NULL ))
// 		PrintLastError(TEXT("StackWalk642"));
// 
// 	stack[stack_idx++] = (void*)stack_frame.AddrPC.Offset;
// 	stack[stack_idx++] = (void*)thread_context_1.Eip;
// 
// 
// 
// 
// 
// 
// 	MEMORY_BASIC_INFORMATION MemInfo;
// 
// 	stack[stack_idx++] = (void*)Error;
// 	stack[stack_idx++] = (void*)Error1;
// 	stack[stack_idx++] = (void*)Error2;
// 	
// 	stack[stack_idx++] = (void*)1;
// 
// 	// VirtualQuery can be used to get the allocation base associated with a
// 	// code address, which is the same as the ModuleHandle. This can be used
// 	// to get the filename of the module that the crash happened in.
// 	if (VirtualQuery((void*)exinfo->ContextRecord->Eip, &MemInfo, sizeof(MemInfo)))
// 	{
// 		stack[stack_idx++] = MemInfo.AllocationBase;
// 	}
// 
// 	stack[stack_idx++] = (void*)1;
// 
// 
// 	dump_stack(exinfo->ContextRecord->Ebp, stack + stack_idx, 30 - stack_idx);
// 
// 	for (int i = 0; stack[i];++i)
// 	{
// 		char buf[20];
// 		sprintf(buf, "0x%08X\r\n", stack[i]);
// 		OutputDebugStringA(buf);
// 	}

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
	return true;
}

int Error()
{
	//throw 0;
	int a = GetTickCount();
	int b = a % 0xFFFFFFFFFF;
	return 1/(b - a);
}

void PrintLastError(LPCTSTR fun_name)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	// Display the error message and exit the process

	TCHAR lpDisplayBuf[1024];

	StringCchPrintf((LPTSTR)lpDisplayBuf, ARRAYSIZE(lpDisplayBuf), TEXT("%s error %d: %s\r\n"), fun_name, dw, lpMsgBuf); 
	OutputDebugString(lpDisplayBuf);

	LocalFree(lpMsgBuf);
}
