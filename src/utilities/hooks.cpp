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
                mov funcs::dmg::temp, eax
                movzx eax, variables::godmode
                cmp eax, 0
                mov eax, funcs::dmg::temp
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
            funcs::dmg::addr = cc::cheat::find_signature(funcs::dmg::sig, funcs::dmg::mask, reinterpret_cast<uintptr_t>(si.lpMinimumApplicationAddress), reinterpret_cast<uintptr_t>(si.lpMaximumApplicationAddress) - reinterpret_cast<uintptr_t>(si.lpMinimumApplicationAddress), 0xff);
            if (funcs::dmg::addr == 0)
                throw std::runtime_error("Unable to find funcs::dmg::addr!");
            console::log() << "funcs::dmg::addr found! - " << std::hex << addr << std::endl;
            
            funcs::dmg::backup = cc::cheat::read_bytes(funcs::dmg::addr + funcs::dmg::offset, 6);
            funcs::dmg::jmp_back = funcs::dmg::addr + funcs::dmg::offset + 6;
            std::vector<uint8_t> bytes = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0x90 };
            cc::cheat::write(reinterpret_cast<uintptr_t>(bytes.data() + 1), reinterpret_cast<uint32_t>(funcs::dmg::hook) - (funcs::dmg::addr + funcs::dmg::offset) - 5);
            cc::cheat::write_bytes(funcs::dmg::addr + funcs::dmg::offset, bytes);
        }

        void release()
        {
            cc::cheat::write_bytes(funcs::dmg::addr + funcs::dmg::offset, funcs::dmg::backup);
        }
    }

    namespace mana
    {
        uintptr_t addr;
        std::vector<uint8_t> backup;
        uint32_t temp;
        uint32_t jmp_back;

        void _declspec(naked) hook()
        {
            _asm
            {
                mov funcs::mana::temp, eax
                movzx eax, variables::mana
                cmp eax, 0
                mov eax, funcs::mana::temp
                jne return
                sub [esi+0x3E8],eax
            return:
                jmp [jmp_back]
            }
        }

        void init()
        {
            SYSTEM_INFO si;
            GetSystemInfo(&si);
            funcs::mana::addr = cc::cheat::find_signature(funcs::mana::sig, funcs::mana::mask, reinterpret_cast<uintptr_t>(si.lpMinimumApplicationAddress), reinterpret_cast<uintptr_t>(si.lpMaximumApplicationAddress) - reinterpret_cast<uintptr_t>(si.lpMinimumApplicationAddress), 0xff);
            if (funcs::mana::addr == 0)
                throw std::runtime_error("Unable to find funcs::mana::addr!");
            console::log() << "funcs::mana::addr found! - " << std::hex << funcs::mana::addr << std::endl;

            funcs::mana::backup = cc::cheat::read_bytes(funcs::mana::addr + funcs::mana::offset, 6);
            funcs::mana::jmp_back = funcs::mana::addr + funcs::mana::offset + 6;
            std::vector<uint8_t> bytes = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0x90 };
            cc::cheat::write(reinterpret_cast<uintptr_t>(bytes.data() + 1), reinterpret_cast<uint32_t>(funcs::mana::hook) - (funcs::mana::addr + funcs::mana::offset) - 5);
            cc::cheat::write_bytes(funcs::mana::addr + funcs::mana::offset, bytes);
        }

        void release()
        {
            cc::cheat::write_bytes(funcs::mana::addr + funcs::mana::offset, funcs::mana::backup);
        }
    }
    
    namespace effects
    {
        uintptr_t addr;
        std::vector<uint8_t> backup;
        uint32_t jmp_back;
        uint32_t temp;

        _declspec(naked) void hook()
        {
            _asm
            {
                mov temp, eax
                mov eax, variables::pets
                mov [esi+0x298], eax
                mov eax, temp
                jmp [jmp_back]
            }
        }

        void init()
        {
            SYSTEM_INFO si;
            GetSystemInfo(&si);
            addr = cc::cheat::find_signature(sig, mask, reinterpret_cast<uintptr_t>(si.lpMinimumApplicationAddress), reinterpret_cast<uintptr_t>(si.lpMaximumApplicationAddress) - reinterpret_cast<uintptr_t>(si.lpMinimumApplicationAddress), 0xff);
            if (addr == 0)
                throw std::runtime_error("Unable to find funcs::effects::addr");
            console::log() << "funcs::effects::addr found! - " << std::hex << addr << std::endl;

            backup = cc::cheat::read_bytes(addr + offset, 10);
            jmp_back = addr + offset + 10;
            std::vector<uint8_t> bytes = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90 };
            cc::cheat::write(reinterpret_cast<uintptr_t>(bytes.data() + 1), reinterpret_cast<uint32_t>(hook) - (addr + offset) - 5);
            cc::cheat::write_bytes(addr + offset, bytes);
        }

        void release()
        {
            cc::cheat::write_bytes(addr + offset, backup);
        }
    }
}

void hooks::initialize()
{
    funcs::dmg::init();
    funcs::mana::init();
    funcs::effects::init();
    console::log() << "Hooks initialized!" << std::endl;
}

void hooks::release()
{
    funcs::dmg::release();
    funcs::mana::release();
    funcs::effects::release();
}
