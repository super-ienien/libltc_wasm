
#include <emscripten/bind.h>
#include "ltc.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(timecode_js) {
    value_object<SMPTETimecode>("SMPTETimecode")
    .field("timezone", &SMPTETimecode::timezone)
	.field("years", &SMPTETimecode::years)
	.field("months", &SMPTETimecode::months)
	.field("days", &SMPTETimecode::days)

	.field("hours", &SMPTETimecode::hours)
	.field("mins", &SMPTETimecode::mins)
	.field("secs", &SMPTETimecode::secs)
	.field("frame", &SMPTETimecode::frame)
    ;

    value_array<std::array<char, 6>>("array_char_6")
        .element(emscripten::index<0>())
        .element(emscripten::index<1>())
        .element(emscripten::index<2>())
        .element(emscripten::index<3>())
        .element(emscripten::index<4>())
        .element(emscripten::index<5>())
        .element(emscripten::index<6>())
        ;
}