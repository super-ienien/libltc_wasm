#include "ltc.h"
#include <emscripten/bind.h>
#include "common_js.h"

using namespace emscripten;

LTC_TV_STANDARD standardJsToEnum (int stdJs) {
    switch (stdJs) {
        case 1:
            return LTC_TV_525_60;
        case 2:
            return LTC_TV_625_50;
        case 3:
            return LTC_TV_1125_60;
        case 4:
            return LTC_TV_FILM_24;
        break;
        default:
            return LTC_TV_625_50;
    }
}

EMSCRIPTEN_BINDINGS(sound_sample_js) {
    value_object<SoundSampleJS>("SoundSampleJS")
    .field("buf_ptr", &SoundSampleJS::buf_ptr)
	.field("size", &SoundSampleJS::size)
    ;
}
