
#include "gmock/gmock.h"
#include  "Soundex.h"



using namespace  ::testing;
class SoundexEncoding :public Test {
public:
	Soundex soundex;

};

TEST_F(SoundexEncoding, RetainsSoleLetterOfOneLetterWord) {
	EXPECT_THAT(soundex.encode("A"), Eq("A000"));
}



TEST_F(SoundexEncoding, PadsWithZerosToEnsureThreeDigits) {
	EXPECT_THAT(soundex.encode("I"), Eq("I000"));
}

TEST_F(SoundexEncoding, ReplacesConsonantsWithAppropriateDigits) {
	EXPECT_THAT(soundex.encode("Ab"), Eq("A100"));
}