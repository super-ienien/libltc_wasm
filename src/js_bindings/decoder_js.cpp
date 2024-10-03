#include "decoder_js.h"
#include "decoder.h"
#include "ltc_frame_js.h"
#include <emscripten/bind.h>
#include <cstdint>

using namespace emscripten;

LTCDecoderJS::LTCDecoderJS(int apv, int queue_len) {
    this->_decoder = ltc_decoder_create(apv, queue_len);
}

LTCDecoderJS::~LTCDecoderJS() {
    ltc_decoder_free(this->_decoder);
    this->_decoder = nullptr;
}

void LTCDecoderJS::write(uintptr_t buf_ptr, size_t size) {
    printf("size %ld\n", size);
    float* buf = reinterpret_cast<float*>(buf_ptr);
    ltc_decoder_write_float(this->_decoder, buf, size, 0);
}

LTCFrameJS LTCDecoderJS::read() {
    LTCFrameJS* frame = new LTCFrameJS();
    LTCFrameExt frameExt;
    if (ltc_decoder_read(this->_decoder, &frameExt) == 1) {
        frame->fill(&frameExt);
    }
    return *frame;
}

void LTCDecoderJS::flushQueue() {
    return ltc_decoder_queue_flush(this->_decoder);
}

int LTCDecoderJS::queueLength() {
    return ltc_decoder_queue_length(this->_decoder);
}

EMSCRIPTEN_BINDINGS(decoder_js) {
  class_<LTCDecoderJS>("LTCDecoderJS")
    .constructor<>()
    .constructor<int>()
    .constructor<int, int>()
    .function("write", &LTCDecoderJS::write, allow_raw_pointers())
    .function("read", &LTCDecoderJS::read, return_value_policy::take_ownership());
    .function("flushQueue", &LTCDecoderJS::flushQueue);
    .function("queueLength", &LTCDecoderJS::queueLength);
};