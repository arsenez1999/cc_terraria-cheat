#include <Windows.h>
#include "utilities/console.hpp"
#include <thread>
#include <chrono>

DWORD __stdcall thread_callback(LPVOID hModule)
{
    console::initialize("CyberCommunity Terraria cheat - DEBUG");
    console::log() << "Welcome dev!" << std::endl;
    while (!GetAsyncKeyState(VK_END))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    console::release();
    FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
    return FALSE;
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
        CreateThread(nullptr, 0, thread_callback, hModule, 0, nullptr);
    return TRUE;
}