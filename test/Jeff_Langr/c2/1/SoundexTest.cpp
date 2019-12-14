
#include "gmock/gmock.h"


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

using namespace  ::testing;

TEST(SoundexEncoding, RetainsSoleLetterOfOneLetterWord) {
	
	Soundex soundex;
	auto encoded=soundex.encode("A");
	EXPECT_THAT(encoded, Eq("A000"));
	
}



TEST(SoundexEncoding, PadsWithZerosToEnsureThreeDigits) {
	Soundex soundex;
	auto encoded = soundex.encode("I");
	ASSERT_THAT(encoded, Eq("I000"));
}