#include <Windows.h>
#include <iostream>
using namespace std;
int main()
{
	STARTUPINFO si;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	
	if (CreateProcess(NULL,"C:\\Program Files (x86)\\Notepad++\\notepad++.exe",NULL,NULL,FALSE,NULL,NULL,NULL,&si,&pi) == TRUE)
	{
		WaitForSingleObject( pi.hProcess, INFINITE );
		TerminateProcess(pi.hProcess,NO_ERROR);	// убрать процесс
	}
	else cout << "ERROR!";

	CloseHandle ( pi.hProcess);
	CloseHandle ( pi.hThread);
		
	return 0;
}


