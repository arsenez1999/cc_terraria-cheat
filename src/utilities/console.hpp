#pragma once
#include <iostream>

class console
{
public:
    static void initialize(const char* title);
    static void release();

    static std::ostream& log();
};