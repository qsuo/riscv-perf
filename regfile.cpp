
#include "regfile.h"

RegFile::Output RegFile::operate(RegFile::Input& input)
{
    Output output = {
        .d1 = reg[input.a1],
        .d2 = reg[input.a2]
    };
    if(input.wb_we)
        reg[input.wb_a] = input.wb_d;

    return output;
}


