#ifndef THREAD_H
#define THREAD_H

#include "types.h"

class DllExport Thread {
public:
    Thread();
    virtual ~Thread();
    
    void start();
protected:
    virtual void onRun() = 0;

	HANDLE hThread;
	DWORD dwThreadId;
private:
    static DWORD WINAPI threadProc(LPVOID lpParam);
};

#endif