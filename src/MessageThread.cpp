#include "MessageThread.h"


void MessageThread::onRun()
{ 
	MSG msg;

	while (GetMessage(&msg, 0, 0, 0))
    {
        onMessage(msg.message, msg.wParam, msg.lParam);
    }
}

void MessageThread::postMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
    PostThreadMessage(dwThreadId, msg, wParam, lParam);
}