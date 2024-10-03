#include <iostream>
#include <string>
#include <ltc.h>
#include "ltc_decoder.h"

TimecodeDecoder::TimecodeDecoder(int apv, int queue_size) {
    this->decoder = ltc_decoder_create(apv, queue_size);
}

int TimecodeDecoder::test() {
    return 1234;
}

TimecodeDecoder::~TimecodeDecoder() {
    ltc_decoder_free(this->decoder);
    this->decoder = nullptr;
}