#ifndef LTC_WASM_H
#define LTC_WASM_H
#include <ltc.h>

class TimecodeDecoder {

    private:
    LTCDecoder* decoder;

    public:
    TimecodeDecoder(int apv = 1920, int queue_size = 32);
    int test();
    ~TimecodeDecoder();
};

#endif