#include <iostream>
#include <vector>
#include <iomanip>

int main (int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: sig-crypt << [c-style signature] [xor_key]" << std::endl;
        return 1;
    }

    uint8_t key = static_cast<uint8_t>(strtoul(argv[2], nullptr, 16));
    std::vector<uint8_t> sig(strlen(argv[1]) / 4);
    for (size_t i = 0, j = 0; i < strlen(argv[1]); i++)
    {
        if(argv[1][i] == '\\' || argv[1][i] == 'x')
            continue;
        sig[j] = static_cast<uint8_t>(strtoul(argv[1] + i, nullptr, 16));
        i++;
        j++;
    }
    for (auto& value : sig)
    {
        value ^= key;
        std::cout << "\\x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned int>(value); 
    }
    std::cout << std::endl;
    return 0;
}