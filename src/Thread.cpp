#include "Thread.h"

DWORD Thread::threadProc(LPVOID lpParam)
{
	((Thread*)lpParam)->onRun();
	return 0;
}

Thread::Thread()
{
	hThread = NULL;
}

Thread::~Thread()
{
	CloseHandle(hThread);
}

void Thread::start()
{
	hThread = CreateThread(NULL, 4096, threadProc, this, 0, &dwThreadId);
}