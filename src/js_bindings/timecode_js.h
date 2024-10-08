#ifndef TIMECODE_JS
#define TIMECODE_JS 1

class SMPTETimecodeJS {
    public:
	std::string timezone;   ///< the timezone 6bytes: "+HHMM" textual representation
	unsigned char years; ///< LTC-date uses 2-digit year 00.99
	unsigned char months; ///< valid months are 1..12
	unsigned char days; ///< day of month 1..31

	unsigned char hours; ///< hour 0..23
	unsigned char mins; ///< minute 0..60
	unsigned char secs; ///< second 0..60
	unsigned char frame; ///< sub-second frame 0..(FPS - 1)

	SMPTETimecodeJS();
	SMPTETimecodeJS(std::string timezone, int years, int months, int days, int hours, int mins, int secs, int frame);
	SMPTETimecodeJS(const SMPTETimecode &tc);
	SMPTETimecode toC();
};

#endif