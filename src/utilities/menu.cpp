#include "menu.hpp"
#include "console.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "../dependencies/kiero/kiero.h"

typedef HRESULT (__stdcall* EndScene)(LPDIRECT3DDEVICE9);

EndScene original_EndScene = nullptr;
HMODULE lib;
bool init = false;

DWORD __stdcall exit_callback(LPVOID)
{
    Sleep(100);
    FreeLibraryAndExitThread(lib, 0);
}

HRESULT __stdcall hook_EndScene(LPDIRECT3DDEVICE9 pDevice)
{
    if (!init)
    {
        console::log() << "EndScene: hooked!" << std::endl;
        init = true;
    }
    if (GetAsyncKeyState(VK_END))
    {
        menu::release();
        console::release();
        CreateThread(nullptr, 0, exit_callback, nullptr, 0, nullptr);
        return 0;
    }
    return original_EndScene(pDevice);
}

void menu::initialize(void* hModule)
{
    lib = reinterpret_cast<HMODULE>(hModule);
    bool attached = false;
    do
    {
        if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
        {
            kiero::bind(42, reinterpret_cast<void**>(&original_EndScene), hook_EndScene);
            attached = true;
        }
    } while (!attached);
}

void menu::release()
{
    kiero::shutdown();
}
