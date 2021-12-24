#include "menu.hpp"
#include "console.hpp"
#include "hooks.hpp"
#include "../vars.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "../dependencies/kiero/kiero.h"
#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_dx9.h"
#include "../dependencies/imgui/imgui_impl_win32.h"

typedef HRESULT (__stdcall* EndScene)(LPDIRECT3DDEVICE9);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

EndScene original_EndScene = nullptr;
LPDIRECT3DDEVICE9 directx_device;
HWND window;
WNDPROC original_WndProc;
HMODULE lib;
bool init = false;
bool menu = false;

DWORD __stdcall exit_callback(LPVOID)
{
    Sleep(100);
    FreeLibraryAndExitThread(lib, 0);
}

void init_imgui(LPDIRECT3DDEVICE9 pDevice)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDrawCursor = true;
    io.IniFilename = nullptr;
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(pDevice);
}

HRESULT __stdcall hook_EndScene(LPDIRECT3DDEVICE9 pDevice)
{
    if (!init)
    {
        console::log() << "EndScene: hooked!" << std::endl;
        directx_device = pDevice;
        init_imgui(pDevice);
        init = true;
    }
    if (GetAsyncKeyState(VK_END))
    {
        menu::release();
        console::release();
        hooks::release();
        CreateThread(nullptr, 0, exit_callback, nullptr, 0, nullptr);
        return 0;
    }
    if (GetAsyncKeyState(VK_HOME) & 1)
        menu = !menu;

    if(pDevice != directx_device)
    {
        directx_device = pDevice;
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplDX9_Init(pDevice);
    }
    if (menu) 
    {
        ImGui_ImplWin32_NewFrame();
        ImGui_ImplDX9_NewFrame();
        ImGui::NewFrame();

        ImGui::Checkbox("God mode", &variables::godmode);
        ImGui::Checkbox("No mana cost", &variables::mana);
        ImGui::Text("Default max pets count");
        ImGui::SliderInt("##", reinterpret_cast<int32_t*>(&variables::pets), 1, 50);

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        ImGui_ImplDX9_InvalidateDeviceObjects();
    }
    return original_EndScene(pDevice);
}

LRESULT __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if(ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;
    return CallWindowProc(original_WndProc, hwnd, msg, wParam, lParam);
}

BOOL __stdcall EnumWindows_callback(HWND handle, LPARAM)
{
    DWORD proc_id;
    GetWindowThreadProcessId(handle, &proc_id);

    if (GetCurrentProcessId() != proc_id || GetConsoleWindow() == handle)
        return TRUE;

    window  = handle;
    return FALSE;
}

HWND get_window()
{
    window = nullptr;
    EnumWindows(EnumWindows_callback, 0);
    return window;
}

void menu::initialize(void* hModule)
{
    lib = reinterpret_cast<HMODULE>(hModule);
    bool attached = false;
    do
    {
        if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
        {
            do
            {
                window = get_window();
            } while (window == nullptr);
            original_WndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWL_WNDPROC, reinterpret_cast<LONG>(WndProc)));
            kiero::bind(42, reinterpret_cast<void**>(&original_EndScene), hook_EndScene);
            attached = true;
        }
    } while (!attached);
}

void menu::release()
{
    SetWindowLongPtr(window, GWL_WNDPROC, reinterpret_cast<LONG>(original_WndProc));
    kiero::shutdown();
}
