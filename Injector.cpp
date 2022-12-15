#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

DWORD _GetProcessId(LPCTSTR ProcessName) // non-conflicting function name
{
	PROCESSENTRY32 pt;
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pt.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hsnap, &pt)) { // must call this first
		do {
			if (!lstrcmpi(pt.szExeFile, ProcessName)) {
				CloseHandle(hsnap);
				return pt.th32ProcessID;
			}
		} while (Process32Next(hsnap, &pt));
	}
	CloseHandle(hsnap); // close handle on failure
	return 0;
}

using namespace std;

int main() {
	char* dllPath = new char;
	char* processName = new char;

	cout << "Enter .dll path\n";
	cin >> dllPath;

	cout << "Enter process name\n";
	cin >> processName;

	DWORD procId = 0;

	int progCircleIdx = 0;

	const char* progCircle = "|/-\\";

	while (!procId) {
		procId = _GetProcessId((LPCTSTR) processName);
		system("cls");
		cout << "Searching for process ";
		cout << progCircle[(progCircleIdx++) % strlen(progCircle)];
		Sleep(60);
	}

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);

	if (hProc && hProc != INVALID_HANDLE_VALUE) {
		void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		if (loc) {
			WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, 0);
		}

		HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

		if (hThread) {
			CloseHandle(hThread);
		}
	}

	if (hProc) {
		CloseHandle(hProc);
	}

	return 0;
}