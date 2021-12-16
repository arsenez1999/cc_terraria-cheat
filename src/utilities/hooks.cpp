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
        std::vector<uint8_t> backup;
        uint32_t temp;
        uint32_t jmp_back;

        void _declspec(naked) hook()
        {
        _asm
        {
            mov temp, eax
            movzx eax, variables::godmode
            cmp eax, 0
            mov eax, temp
            jne return
            sub [edx+0x3E4], eax
        return:
            jmp [jmp_back]
        }
        }

        void init()
        {
            SYSTEM_INFO si;
            GetSystemInfo(&si);
            addr = cc::cheat::find_signature(funcs::dmg::sig, funcs::dmg::mask, reinterpret_cast<uintptr_t>(si.lpMinimumApplicationAddress), reinterpret_cast<uintptr_t>(si.lpMaximumApplicationAddress) - reinterpret_cast<uintptr_t>(si.lpMinimumApplicationAddress), 0xff);
            if (addr == 0)
                throw std::runtime_error("Unable to find funcs::dmg::addr!");
            console::log() << "funcs::dmg::addr found! - " << std::hex << addr << std::endl;
            
            backup = cc::cheat::read_bytes(funcs::dmg::addr + funcs::dmg::offset, 6);
            jmp_back = addr + offset + 6;
            std::vector<uint8_t> bytes = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0x90 };
            cc::cheat::write(reinterpret_cast<uintptr_t>(bytes.data() + 1), reinterpret_cast<uint32_t>(hook) - (funcs::dmg::addr + funcs::dmg::offset) - 5);
            cc::cheat::write_bytes(funcs::dmg::addr + funcs::dmg::offset, bytes);
        }

        void release()
        {
            cc::cheat::write_bytes(funcs::dmg::addr + funcs::dmg::offset, backup);
        }
    }
}

void hooks::initialize()
{
    funcs::dmg::init();
    console::log() << "Hooks initialized!" << std::endl;
}

void hooks::release()
{
    funcs::dmg::release();
}