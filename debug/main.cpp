#include <windows.h>

#include <stdio.h>

BOOL createChildProcess(LPSTR strCmdLine, HANDLE& hProcess, HANDLE& hPipe)
{ 

	HANDLE hSaveStdout;
	HANDLE hChildStdoutRd;
	HANDLE hChildStdoutWr;
	//HANDLE hChildStdoutRdDup;
	SECURITY_ATTRIBUTES saAttr; 
	BOOL fSuccess; 

	// Set the bInheritHandle flag so pipe handles are inherited. 

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 

	// The steps for redirecting child process's STDOUT: 
	//     1. Save current STDOUT, to be restored later. 
	//     2. Create anonymous pipe to be STDOUT for child process. 
	//     3. Set STDOUT of the parent process to be write handle to 
	//        the pipe, so it is inherited by the child process. 
	//     4. Create a noninheritable duplicate of the read handle and
	//        close the inheritable read handle. 

	// Save the handle to the current STDOUT. 

	hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE); 

	// Create a pipe for the child process's STDOUT. 

	if (! CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)) 
	  return FALSE; 

	// Set a write handle to the pipe to be STDOUT. 

	SetStdHandle(STD_OUTPUT_HANDLE, hChildStdoutWr);

	// Create noninheritable read handle and close the inheritable read 
	// handle. 

	fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdoutRd,
		GetCurrentProcess(), &hPipe , 0,
		FALSE,
		DUPLICATE_SAME_ACCESS);

	if( !fSuccess )
		return FALSE;

	CloseHandle(hChildStdoutRd);


// After process creation, restore the saved STDIN and STDOUT. 



   PROCESS_INFORMATION piProcInfo; 
   STARTUPINFO siStartInfo; 

// Set up members of the PROCESS_INFORMATION structure. 

   ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

// Set up members of the STARTUPINFO structure. 

   ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
   siStartInfo.cb = sizeof(STARTUPINFO); 

// Create the child process. 

   fSuccess = CreateProcess(NULL, 
	  strCmdLine,       // command line 
	  NULL,          // process security attributes 
	  NULL,          // primary thread security attributes 
	  TRUE,          // handles are inherited 
	  0,             // creation flags 
	  NULL,          // use parent's environment 
	  NULL,          // use parent's current directory 
	  &siStartInfo,  // STARTUPINFO pointer 
	  &piProcInfo);  // receives PROCESS_INFORMATION 

	if( !fSuccess )
		return FALSE;

	hProcess = piProcInfo.hProcess;

	SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout);

	CloseHandle(hChildStdoutWr);

	return TRUE;
}


int main(int argc, char*argv[])
{
    if (argc != 2) {
        printf("Usage: debug [exeName]\n");
        return -1;
    }
    
	HANDLE hPipe;
	CHAR chBuf[1024]; 
	DWORD dwRead;
    HANDLE hProcess;

	//SetCurrentDirectory(strPathName);
	createChildProcess(argv[1], hProcess, hPipe);
	if (!hProcess)
	{
		printf("fail to launch child process\n");
		return -1;
	}

	for(;;)
	{
		if( !ReadFile( hPipe, chBuf, sizeof(chBuf), &dwRead, NULL) || dwRead == 0) 
			break; 
		chBuf[dwRead] = 0;
        printf("%s\n", chBuf);
	}


    return 0;
}