
#include <emscripten/bind.h>
#include "ltc.h"

EMSCRIPTEN_BINDINGS(timecode_js) {
    value_object<SMPTETimecode>("SMPTETimecode")
    .filed("timezone", &SMPTETimecode::timezone)   ///< the timezone 6bytes: "+HHMM" textual representation
	.filed("years", &SMPTETimecode::years) ///< LTC-date uses 2-digit year 00.99
	.filed("months", &SMPTETimecode::months) ///< valid months are 1..12
	.filed("days", &SMPTETimecode::days) ///< day of month 1..31

	.filed("hours", &SMPTETimecode::hours) ///< hour 0..23
	.filed("mins", &SMPTETimecode::mins) ///< minute 0..60
	.filed("secs", &SMPTETimecode::secs) ///< second 0..60
	.filed("frame", &SMPTETimecode::frame) ///< sub-second frame 0..(FPS - 1)
    ;

    value_array<std::array<char, 6>>("array_char_6")
        .element(index<0>())
        .element(index<1>())
        .element(index<2>())
        .element(index<3>())
        .element(index<4>())
        .element(index<5>())
        .element(index<6>())
        ;
}