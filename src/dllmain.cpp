#include <Windows.h>
#include "utilities/console.hpp"
#include "utilities/menu.hpp"
#include "utilities/hooks.hpp"
#include <thread>
#include <chrono>

DWORD __stdcall thread_callback(LPVOID hModule)
{
#ifndef NDEBUG
    console::initialize("CyberCommunity Terraria cheat - DEBUG");
    console::log() << "Welcome dev!" << std::endl;
#endif
    try
    {
        menu::initialize(hModule);
        hooks::initialize();
    }
    catch(const std::exception& e)
    {
        console::log() << e.what() << '\n';
    }
    
    return TRUE;
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
        CreateThread(nullptr, 0, thread_callback, hModule, 0, nullptr);
    return TRUE;
}