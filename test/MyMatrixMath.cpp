
#include "gtest/gtest.h"



namespace  mymm {

	template<typename T,int R,int C>
	class mat {
	private:
		T data[R*C];

	public:
		mat() {
			for (int i = 0; i < R*C; i++) {
				data[i] = 0;
			}
		}
		mat(T v) {
			for (int i = 0; i < R*C; i++) {
				data[i] = v;
			}
		}

		const T& operator[](const int i) const {
			return data[i];
		}

		T& operator[](const int i) {
			return data[i];
		}

		const mat& operator+=(mat & other) {
			for (int i = 0; i < R*C; i++) {
				data[i] += other[i];
			}
			return *this;
		}

	};








}



TEST(MyMatrixMath, test_aaa) {

	mymm::mat<float, 3, 3> a;
	a[0] = 1;
	a[1] = 2;

	mymm::mat<float, 3, 3> b(1);
	a += b;

	mymm::mat<float, 3, 3> c(1);


}