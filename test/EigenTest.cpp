
#include "gtest/gtest.h"

#include "Eigen/Dense"
#include <iostream>

using Eigen::MatrixXd;

TEST(EigenTest, test_std_vector) {
	std::vector<Eigen::Vector2f> x{ {1,1},{1,1} };
	std::vector<Eigen::Vector2f> x1{ {2,2},{2,2} };


	std::vector<Eigen::Vector2f> sum_act;
	for (int i = 0; i < x.size(); i++) {
		sum_act.push_back(x[i] + x1[i]);
	}

	std::vector<Eigen::Vector2f> sum_exp{ {3,3},{3,3} };
	EXPECT_EQ(sum_act, sum_exp);

}

TEST(EigenTest, test_memory_is_continuous) {
	std::vector<Eigen::Vector2f> x{ {1,2},{3,4} };

	std::vector<float> x_act(&x[0][0], &x[0][0] + 4);

	std::vector<float> x_exp{1,2,3,4 };



	EXPECT_EQ(x_act, x_exp);

}
