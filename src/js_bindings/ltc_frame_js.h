#ifndef LTC_FRAME_JS
#define LTC_FRAME_JS 1

#include "ltc.h"

// float getBiphaseTics(LTCFrameExt f) {
//     return f.biphase_tics;
// }

class LTCFrameJS {
    public:
    unsigned int user1;
    unsigned int frame_units;
    unsigned int user2;
    unsigned int col_frame;
    unsigned int dfbit;
    unsigned int frame_tens;
    unsigned int user3;
    unsigned int secs_units;
    unsigned int user4;
    unsigned int biphase_mark_phase_correction;
    unsigned int secs_tens;
    unsigned int user5;
    unsigned int mins_tens;
    unsigned int mins_units;
    unsigned int user6;
    unsigned int binary_group_flag_bit0;
    unsigned int hours_units;
    unsigned int user7;
    unsigned int user8;
    unsigned int binary_group_flag_bit2;
    unsigned int binary_group_flag_bit1;
    unsigned int hours_tens;
    unsigned int sync_word;

    bool is_valid;
// 	LTCFrame ltc; ///< the actual LTC frame. see \ref LTCFrame
	ltc_off_t off_start; ///< \anchor off_start the approximate sample in the stream corresponding to the start of the LTC frame.
	ltc_off_t off_end; ///< \anchor off_end the sample in the stream corresponding to the end of the LTC frame.
	int reverse; ///< if non-zero, a reverse played LTC frame was detected. Since the frame was reversed, it started at off_end and finishes as off_start (off_end > off_start). (Note: in reverse playback the (reversed) sync-word of the next/previous frame is detected, this offset is corrected).
// 	float biphase_tics[LTC_FRAME_BIT_COUNT]; ///< detailed timing info: phase of the LTC signal; the time between each bit in the LTC-frame in audio-frames. Summing all 80 values in the array will yield audio-frames/LTC-frame = (\ref off_end - \ref off_start + 1).
	ltcsnd_sample_t sample_min; ///< the minimum input sample signal for this frame (0..255)
	ltcsnd_sample_t sample_max; ///< the maximum input sample signal for this frame (0..255)
	double volume; ///< the volume of the input signal in dbFS

    LTCFrameJS();

    void fill(LTCFrameExt* frame);
};

#endif