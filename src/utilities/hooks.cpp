#include "hooks.hpp"
#include "console.hpp"
#include "../offsets.hpp"
#include "../vars.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <internal.hpp>
#include <stdexcept>

namespace funcs
{
    //Terraria.Player::Hurt
    namespace dmg
    {
        uintptr_t addr;

        void init()
        {
            SYSTEM_INFO si;
            GetSystemInfo(&si);
            addr = cc::cheat::find_signature(funcs::dmg::sig, funcs::dmg::mask, reinterpret_cast<uintptr_t>(si.lpMinimumApplicationAddress), reinterpret_cast<uintptr_t>(si.lpMaximumApplicationAddress) - reinterpret_cast<uintptr_t>(si.lpMinimumApplicationAddress), 0xff);
            if (addr == 0)
                throw std::runtime_error("Unable to find funcs::dmg::addr!");
            console::log() << "funcs::dmg::addr found! - " << std::hex << addr << std::endl;
            
        }

        void release()
        {

        }
    }
}

void hooks::initialize()
{
    funcs::dmg::init();
}

void hooks::release()
{
    funcs::dmg::release();
}