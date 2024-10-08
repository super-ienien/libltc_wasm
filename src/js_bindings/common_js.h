#ifndef COMMON_JS
#define COMMON_JS 1

#include "ltc.h"
#include <emscripten/bind.h>

LTC_TV_STANDARD standardJsToEnum (int stdJs);

struct SoundSampleJS {
    uintptr_t buf_ptr;
    size_t size;
};

#endif

