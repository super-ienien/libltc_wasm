#ifndef LTC_ENCODER_JS
#define LTC_ENCODER_JS 1

#include <emscripten/val.h>
#include "encoder.h"
#include "ltc_frame_js.h"
#include <cstdint>
#include "ltc.h"

using namespace emscripten;

class LTCEncoderJS {
    private:
    LTCEncoder* _encoder;
    SMPTETimecode tc;

    public:
    LTCEncoderJS(double sample_rate, double fps, LTC_TV_STANDARD standard, int flags );
    ~LTCEncoderJS();
    int decrementTimecode();
    int incrementTimecode();
    int encodeByte(int byte, double speed);
    void encodeFrame();
    void encodeReversedFrame();
    int endEncode();
    double getFilter();
    void setFilter(double rise_time);
//     LTCFrameJS getFrame();
//     LTCFrameJS setFrame(LTCFrameJS frame);
    SMPTETimecode getTimecode();
    void setTimecode(SMPTETimecode timecode);
    double getVolume();
    int setVolume(double dBFS);
    int reinit(double sample_rate, double fps, LTC_TV_STANDARD standard, int flags );
    void reset();
    void setUserBits(unsigned long data);
    int getBufferSize();
    int setBufferSize(double sample_rate, double fps);
    val getBuffer(bool flush);
};

#endif