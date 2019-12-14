#ifndef SOUNDEX_H
#define SOUNDEX_H


#include <string>

class Soundex {

public:
	std::string encode(const std::string& str) {
		return zeroPad(head(str) + encodedDigits(str));

	}
private:


	std::string head(const std::string& str) {
		return str.substr(0, 1);
	}
	std::string encodedDigits(const std::string& str) {
		if (str.length() > 1) {
			return "1";
		}
		else {
			return "";
		}
	}

	std::string zeroPad(const std::string& str) {
		auto zerosNeeded = 4 - str.length();
		return str+std::string(zerosNeeded,'0');
	}
};


#endif
