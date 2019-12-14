#ifndef SOUNDEX_H
#define SOUNDEX_H


#include <string>
#include <unordered_map>

class Soundex {

public:
	std::string encode(const std::string& str) {
		return zeroPad(head(str) + encodedDigits(tail(str)));

	}
private:


	std::string head(const std::string& str) {
		return str.substr(0, 1);
	}

	std::string tail(const std::string& str) {
		return str.substr( 1);
	}





	std::string encodedDigits(const std::string& str) {
		std::string encoding;
		for (auto letter : str) {
			encoding += encodedDigit(letter);
			if (isComplete(encoding)) {
				break;
			}
		}
		return encoding;
	}

	bool isComplete(const std::string& str) {
		return str.length() == MaxCodeLength - 1;
	}

	std::string encodedDigit(const char c) const {
		std::unordered_map<char, std::string> encodings{
		 {'b', "1"}, {'f', "1"}, {'p', "1"}, {'v', "1"},
		 {'c', "2"}, {'g', "2"}, {'j', "2"}, {'k', "2"}, {'q', "2"},
					 {'s', "2"}, {'x', "2"}, {'z', "2"},
		 {'d', "3"}, {'t', "3"},
		 {'l', "4"},
		 {'m', "5"}, {'n', "5"},
		 {'r', "6"}
		};
		auto it = encodings.find(c);

		return it == encodings.end() ? "" : it->second;
	}

	std::string zeroPad(const std::string& str) {
		auto zerosNeeded = MaxCodeLength - str.length();
		return str+std::string(zerosNeeded,'0');
	}


	static const size_t MaxCodeLength{ 4 };


};


#endif
