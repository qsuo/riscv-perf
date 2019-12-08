
#ifndef REGFILE_H
#define REGFILE_H

#include <cstddef>

using data_t = uint32_t;

class RegFile
{
public:

    struct Input
    {
        uint8_t a1;
        uint8_t a2;
        uint8_t a3;
        uint8_t wb_a;
        uint32_t wb_d;
        bool wb_we;
    }

    struct Output
    {
        uint32_t d1;
        uint32_t d2;
    }

    Output operate(Input& input);

    const int XLEN = 32;

private:

    uint32_t reg[XLEN];
};

#endif
