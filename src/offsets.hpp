#pragma once

namespace funcs
{
    //Terraria.Player::Hurt
    namespace dmg
    {
        constexpr char* sig = "\xaa\x74\x13\xa8\xa9\xac\x7e\x13\x8f\xfd\xff\xff\x74\x0e\x72\x42\xff\xff\xff\xff\x46\xff\xff\xff\xff\xcc\x3f\x0c\xff\x74\x31\x76\x72";
        constexpr char* mask = "xxxxxxxxxxxxxxxx????x????xxx?xxxx";
        constexpr uint32_t offset = 0x1177;
    }
}