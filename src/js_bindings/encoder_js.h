#ifndef LTC_ENCODER_JS
#define LTC_ENCODER_JS 1

#include <emscripten/val.h>
#include "encoder.h"
#include "ltc_frame_js.h"
#include "timecode_js.h"
#include <cstdint>
#include "ltc.h"
#include "common_js.h"

using namespace emscripten;

class LTCEncoderJS {
    private:
    LTCEncoder* _encoder;
    SMPTETimecode tc;

    public:
    LTCEncoderJS(double sample_rate, double fps, int standard, int flags = 0);
    ~LTCEncoderJS();
    int decrementTimecode();
    int incrementTimecode();
    int encodeByte(int byte, double speed);
    void encodeFrame();
    void encodeReversedFrame();
    int endEncode();
    double getFilter();
    void setFilter(double rise_time);
    SMPTETimecodeJS* getTimecode();
    void setTimecode(SMPTETimecodeJS timecode);
    double getVolume();
    int setVolume(double dBFS);
    int reinit(double sample_rate, double fps, int standard, int flags = 0);
    void reset();
    void setUserBits(unsigned long data);
    int getBufferSize();
    int setBufferSize(double sample_rate, double fps);
    SoundSampleJS getBuffer();
    void flushBuffer();
};

#endif