
#include "gmock/gmock.h"


namespace template_concept {

	template<typename T>
	concept geoObj = require(T x) = {
		{x.draw()}->void;
	};


}

using namespace testing;
using namespace template_concept;


TEST(conceptaa, use)
{



}