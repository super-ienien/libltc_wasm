#ifndef LTC_DECODER_JS
#define LTC_DECODER_JS 1

#include "decoder.h"
#include "common_js.h"
#include "ltc_frame_js.h"
#include <cstdint>

class LTCDecoderJS {
    private:
    LTCDecoder* _decoder;

    public:
    LTCDecoderJS(int apv = 1920, int queue_len = 32);
    ~LTCDecoderJS();
    void write(SoundSampleJS sample);
    void writeBytes(SoundSampleJS sample);
    LTCFrameJS read();
    void flushQueue();
    int queueLength();
};

#endif