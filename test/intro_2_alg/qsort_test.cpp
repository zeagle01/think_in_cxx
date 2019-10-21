


#include "gtest/gtest.h" 




namespace QSort {

	int split(int* A, const int p,const int r) {
		int x = A[r];
		int i = p-1;
		for (int j = p; j <= r - 1;j++) {
			if (A[j] <= x) {
				i++;
				std::swap(A[i], A[j]);
			}
		}
		std::swap(A[i+1], A[r]);
		return i+1;

	}

	void qsort(int* A, const int p,const int r) {
		if (p < r) {
			int q = split(A, p, r);
			qsort(A, p, q - 1);
			qsort(A, q + 1, r);
		}
	}

}






TEST(QSort_Test,test_321) {

	std::vector<int> A{ 3,2,1 };

	QSort::qsort( &A[0], 0, A.size()-1);

	std::vector<int> exp{ 1,2,3 };

	EXPECT_EQ(A, exp);
}

TEST(QSort_Test,test_213) {

	std::vector<int> A{ 2,1,3 };

	QSort::qsort( &A[0], 0, A.size()-1);

	std::vector<int> exp{ 1,2,3 };

	EXPECT_EQ(A, exp);
}

TEST(QSort_Test,test_3212) {

	std::vector<int> A{ 3,2,1,2 };

	QSort::qsort( &A[0], 0, A.size()-1);

	std::vector<int> exp{ 1,2,2,3 };

	EXPECT_EQ(A, exp);
}
