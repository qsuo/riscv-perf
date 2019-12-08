
#ifndef MUX_H
#define MUX_H

template <int Tsize>
class Mux
{
public:

    struct Input
    {
        int a;
        uint32_t in1;
        uint32_t in2;
    };

    struct Output
    {
        uint32_t d;
    };

    Output operate(Input& input)
    {
        Output output;
        if(input.a == 0)
            output.d = input.in1;
        else
            output.d = input.in2;
        return output;
    }

};

#endif
