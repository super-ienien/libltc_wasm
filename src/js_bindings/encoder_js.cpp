#include "encoder_js.h"
#include "decoder.h"
#include "common_js.h"
#include "ltc.h"
#include "ltc_frame_js.h"
#include "timecode_js.h"
#include <emscripten/bind.h>
#include <cstdint>
#include <emscripten/val.h>

using namespace emscripten;

LTCEncoderJS::LTCEncoderJS(double sample_rate, double fps, int standard, int flags) {
    this->_encoder = ltc_encoder_create(sample_rate, fps, standardJsToEnum(standard), flags);
}

LTCEncoderJS::~LTCEncoderJS() {
    ltc_encoder_free(this->_encoder);
    this->_encoder = nullptr;
}

int LTCEncoderJS::decrementTimecode() {
    return ltc_encoder_dec_timecode(this->_encoder);
}

int LTCEncoderJS::incrementTimecode() {
    return ltc_encoder_inc_timecode(this->_encoder);
}

int LTCEncoderJS::getBufferSize() {
    return ltc_encoder_get_buffersize(this->_encoder);
}

int LTCEncoderJS::encodeByte(int byte, double speed) {
    return ltc_encoder_encode_byte(this->_encoder, byte, speed);
}

void LTCEncoderJS::encodeFrame() {
    return ltc_encoder_encode_frame(this->_encoder);
}

void LTCEncoderJS::encodeReversedFrame() {
    return ltc_encoder_encode_reversed_frame(this->_encoder);
}

int LTCEncoderJS::endEncode() {
    return ltc_encoder_end_encode(this->_encoder);
}

double LTCEncoderJS::getFilter() {
    return ltc_encoder_get_filter(this->_encoder);
}

void LTCEncoderJS::setFilter(double rise_time) {
    return ltc_encoder_set_filter(this->_encoder, rise_time);
}

SMPTETimecodeJS* LTCEncoderJS::getTimecode() {
    ltc_encoder_get_timecode(this->_encoder, &this->tc);
    return new SMPTETimecodeJS(this->tc);
}

void LTCEncoderJS::setTimecode(SMPTETimecodeJS timecode) {
    SMPTETimecode tc = timecode.toC();
    ltc_encoder_set_timecode(this->_encoder, &tc);
}

double LTCEncoderJS::getVolume() {
    return ltc_encoder_get_volume(this->_encoder);
}

int LTCEncoderJS::setVolume(double dBFS) {
    return ltc_encoder_set_volume(this->_encoder, dBFS);
}

int LTCEncoderJS::reinit(double sample_rate, double fps, int standard, int flags ) {
    return ltc_encoder_reinit(this->_encoder, sample_rate, fps, standardJsToEnum(standard), flags);
}

void LTCEncoderJS::reset() {
    ltc_encoder_reset(this->_encoder);
}

void LTCEncoderJS::setUserBits(unsigned long data) {
    ltc_encoder_set_user_bits(this->_encoder, data);
}

int LTCEncoderJS::setBufferSize(double sample_rate, double fps) {
    return ltc_encoder_set_buffersize(this->_encoder, sample_rate, fps);
}

SoundSampleJS LTCEncoderJS::getBuffer() {
    ltcsnd_sample_t *buf;
    int len = ltc_encoder_copy_buffer(this->_encoder, buf);
    SoundSampleJS sample;
    sample.buf_ptr = (uintptr_t)(void*)buf;
    sample.size = len;
    return sample;
}

void LTCEncoderJS::flushBuffer() {
    ltc_encoder_buffer_flush(this->_encoder);
}

EMSCRIPTEN_BINDINGS(encoder_js) {
  class_<LTCEncoderJS>("LTCEncoderJS")
    .constructor<double, double, int, int>()
    .function("decrementTimecode", &LTCEncoderJS::decrementTimecode)
    .function("incrementTimecode", &LTCEncoderJS::incrementTimecode)
    .function("getBufferSize", &LTCEncoderJS::getBufferSize)
    .function("encodeByte", &LTCEncoderJS::encodeByte)
    .function("encodeFrame", &LTCEncoderJS::encodeFrame)
    .function("encodeReversedFrame", &LTCEncoderJS::encodeReversedFrame)
    .function("endEncode", &LTCEncoderJS::endEncode)
    .function("getFilter", &LTCEncoderJS::getFilter)
    .function("setFilter", &LTCEncoderJS::setFilter)
    .function("getTimecode", &LTCEncoderJS::getTimecode, return_value_policy::take_ownership())
    .function("setTimecode", &LTCEncoderJS::setTimecode)
    .function("getVolume", &LTCEncoderJS::getVolume)
    .function("setVolume", &LTCEncoderJS::setVolume)
    .function("reinit", &LTCEncoderJS::reinit)
    .function("reset", &LTCEncoderJS::reset)
    .function("setUserBits", &LTCEncoderJS::setUserBits)
    .function("setBufferSize", &LTCEncoderJS::setBufferSize)
    .function("getBuffer", &LTCEncoderJS::getBuffer)
    .function("flushBuffer", &LTCEncoderJS::flushBuffer)
    ;
};