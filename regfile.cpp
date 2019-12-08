
#include "regfil.h"

RegFile::operate(Input& input)
{
    Output output = {
        .d1 = reg[input.a1],
        .d2 = reg[input.a2]
    };
    if(wb_we)
        reg[input.wb_a] = input.wb_d;

    return output;
}


