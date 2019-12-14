#ifndef SOUNDEX_H
#define SOUNDEX_H


#include <string>

class Soundex {

public:
	std::string encode(const std::string& str) {
		return zeroPad(str);
	}
private:
	std::string zeroPad(const std::string& str) {
		return str+"000";
	}
};


#endif
