#include <emscripten/bind.h>
#include "ltc_frame_js.h"
#include "ltc.h"

using namespace emscripten;

// float getBiphaseTics(LTCFrameExt f) {
//     return f.biphase_tics;
// }

LTCFrameJS::LTCFrameJS() {
    this->is_valid = false;
};

void LTCFrameJS::fill(LTCFrameExt* frame) {
    this->frame_units = frame->ltc.frame_units;
    this->frame_tens = frame->ltc.frame_tens;
    this->secs_units = frame->ltc.secs_units;
    this->secs_tens = frame->ltc.secs_tens;
    this->mins_units = frame->ltc.mins_units;
    this->mins_tens = frame->ltc.mins_tens;
    this->hours_units = frame->ltc.hours_units;
    this->hours_tens = frame->ltc.hours_tens;
    this->col_frame = frame->ltc.col_frame;
    this->dfbit = frame->ltc.dfbit;
    this->user1 = frame->ltc.user1;
    this->user2 = frame->ltc.user2;
    this->user3 = frame->ltc.user3;
    this->user4 = frame->ltc.user4;
    this->user5 = frame->ltc.user5;
    this->user6 = frame->ltc.user6;
    this->user7 = frame->ltc.user7;
    this->user8 = frame->ltc.user8;
    this->biphase_mark_phase_correction = frame->ltc.biphase_mark_phase_correction;
    this->binary_group_flag_bit0 = frame->ltc.binary_group_flag_bit0;
    this->binary_group_flag_bit1 = frame->ltc.binary_group_flag_bit1;
    this->binary_group_flag_bit2 = frame->ltc.binary_group_flag_bit2;
    this->sync_word = frame->ltc.sync_word;
    this->off_start = frame->off_start;
    this->off_end = frame->off_end;
    this->sample_min = frame->sample_min;
    this->sample_max = frame->sample_max;
    this->volume = frame->volume;
    this->reverse = frame->reverse;
    this->is_valid = true;
};

EMSCRIPTEN_BINDINGS(ltc_frame_js) {
  class_<LTCFrameJS>("LTCFrameJS")
    .constructor<>()
    .property("user1", &LTCFrameJS::user1)
    .property("frame_units", &LTCFrameJS::frame_units)
    .property("user2", &LTCFrameJS::user2)
    .property("col_frame", &LTCFrameJS::col_frame)
    .property("dfbit", &LTCFrameJS::dfbit)
    .property("frame_tens", &LTCFrameJS::frame_tens)
    .property("user3", &LTCFrameJS::user3)
    .property("secs_units", &LTCFrameJS::secs_units)
    .property("user4", &LTCFrameJS::user4)
    .property("biphase_mark_phase_correction", &LTCFrameJS::biphase_mark_phase_correction)
    .property("secs_tens", &LTCFrameJS::secs_tens)
    .property("user5", &LTCFrameJS::user5)
    .property("mins_units", &LTCFrameJS::mins_units)
    .property("user6", &LTCFrameJS::user6)
    .property("binary_group_flag_bit0", &LTCFrameJS::binary_group_flag_bit0)
    .property("mins_tens", &LTCFrameJS::mins_tens)
    .property("user7", &LTCFrameJS::user7)
    .property("hours_units", &LTCFrameJS::hours_units)
    .property("user8", &LTCFrameJS::user8)
    .property("binary_group_flag_bit2", &LTCFrameJS::binary_group_flag_bit2)
    .property("binary_group_flag_bit1", &LTCFrameJS::binary_group_flag_bit1)
    .property("hours_tens", &LTCFrameJS::hours_tens)
    .property("sync_word", &LTCFrameJS::sync_word)
    .property("off_start", &LTCFrameJS::off_start)
    .property("off_end", &LTCFrameJS::off_end)
    .property("reverse", &LTCFrameJS::reverse)
    .property("sample_min", &LTCFrameJS::sample_min)
    .property("sample_max", &LTCFrameJS::sample_max)
    .property("volume", &LTCFrameJS::volume)
    .property("is_valid", &LTCFrameJS::is_valid);
}