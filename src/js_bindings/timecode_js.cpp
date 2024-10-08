
#include <emscripten/bind.h>
#include <string.h>
#include "ltc.h"
#include "timecode_js.h"

using namespace emscripten;

SMPTETimecodeJS::SMPTETimecodeJS() {
}

SMPTETimecodeJS::SMPTETimecodeJS(std::string timezone, int years, int months, int days, int hours, int mins, int secs, int frame) {
    this->timezone = std::string(timezone);
    this->years = (int) years;
    this->months = (int) months;
    this->days = (int) days;
    this->hours = (int) hours;
    this->mins = (int) mins;
    this->years = (int) years;
    this->secs = (int) secs;
    this->frame = (int) frame;
}

SMPTETimecodeJS::SMPTETimecodeJS(const SMPTETimecode &tc) {
    this->timezone = std::string(tc.timezone);
    this->years = (int) tc.years;
    this->months = (int) tc.months;
    this->days = (int) tc.days;
    this->hours = (int) tc.hours;
    this->mins = (int) tc.mins;
    this->years = (int) tc.years;
    this->secs = (int) tc.secs;
    this->frame = (int) tc.frame;
}

SMPTETimecode SMPTETimecodeJS::toC() {
    SMPTETimecode r;
    strcpy(r.timezone, this->timezone.c_str());
    r.years = (unsigned char) this->years;
    r.months = (unsigned char) this->months;
    r.days = (unsigned char) this->days;
    r.hours = (unsigned char) this->hours;
    r.mins = (unsigned char) this->mins;
    r.years = (unsigned char) this->years;
    r.secs = (unsigned char) this->secs;
    r.frame = (unsigned char) this->frame;
    return r;
}

EMSCRIPTEN_BINDINGS(timecode_js) {
    class_<SMPTETimecodeJS>("SMPTETimecodeJS")
    .constructor<>()
    .constructor<std::string, int, int, int, int, int, int, int>()
    .property("timezone", &SMPTETimecodeJS::timezone)
	.property("years", &SMPTETimecodeJS::years)
	.property("months", &SMPTETimecodeJS::months)
	.property("days", &SMPTETimecodeJS::days)

	.property("hours", &SMPTETimecodeJS::hours)
	.property("mins", &SMPTETimecodeJS::mins)
	.property("secs", &SMPTETimecodeJS::secs)
	.property("frame", &SMPTETimecodeJS::frame)
    ;

//     value_array<std::array<char, 6>>("array_char_6")
//         .element(emscripten::index<0>())
//         .element(emscripten::index<1>())
//         .element(emscripten::index<2>())
//         .element(emscripten::index<3>())
//         .element(emscripten::index<4>())
//         .element(emscripten::index<5>())
//         ;
}