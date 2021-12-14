#include "console.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

void console::initialize(const char* title)
{
    AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    SetConsoleTitleA(title);
}

void console::release()
{
    fclose(stdout);
    FreeConsole();
}

std::ostream &console::log()
{
    return std::cout;
}