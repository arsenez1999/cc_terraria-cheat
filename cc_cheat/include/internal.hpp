#pragma once
#include <cstdint>
#include <vector>

namespace cc
{
    namespace cheat
    {
        template<typename T>
        T read(std::uintptr_t addr)
        {
            return *(reinterpret_cast<T*>(addr));
        }

        template<typename T>
        void write(std::uintptr_t addr, const T& value)
        {
            *(reinterpret_cast<T*>(addr)) = value;
        }

        std::uintptr_t find_signature(const char* sig, const char* mask, std::uintptr_t begin, std::uintptr_t size, uint8_t xor_key);

        void write_bytes(std::uintptr_t addr, std::vector<uint8_t> bytes);

        std::vector<uint8_t> read_bytes(std::uintptr_t addr, size_t size);
    }
}