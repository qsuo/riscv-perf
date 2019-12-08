
#ifndef REGFILE_H
#define REGFILE_H

#include <cstddef>


class RegFile
{
public:

    struct Input
    {
        uint32_t a1;
        uint32_t a2;
        uint32_t a3;
        uint32_t wb_a;
        uint32_t wb_d;
        uint32_t wb_we;
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
