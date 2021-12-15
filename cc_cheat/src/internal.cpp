#include "../include/internal.hpp"
#include <cstring>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

std::uintptr_t basic_scan(const char *sig, const char *mask, uintptr_t begin, uintptr_t size, uint8_t xor_key)
{
    size_t sig_len = strlen(mask);
    for (size_t i = 0; i < size; i++)
    {
        bool found = true;
        for (size_t j = 0; j < sig_len; j++)
        {
            if (mask[j] != '?' && cc::cheat::read<uint8_t>(begin + i + j) != static_cast<uint8_t>(sig[j] ^ xor_key))
            {
                found = false;
                break;
            }
        }
        if (found)
            return begin + i;
    }
    return 0;
}

uintptr_t cc::cheat::find_signature(const char *sig, const char *mask, uintptr_t begin, uintptr_t size, uint8_t xor_key)
{
    uintptr_t ret = 0;
    MEMORY_BASIC_INFORMATION mbi;
    for (uintptr_t current = begin; current < begin + size; current += mbi.RegionSize)
    {
        if (!VirtualQuery(reinterpret_cast<void*>(current), &mbi, sizeof(mbi)) || mbi.State != MEM_COMMIT || mbi.Protect & (PAGE_NOACCESS | PAGE_NOCACHE | PAGE_GUARD))
            continue;
        ret = basic_scan(sig, mask, current, mbi.RegionSize, xor_key);
        if (ret != 0)
            break;
    }
    return ret;
}