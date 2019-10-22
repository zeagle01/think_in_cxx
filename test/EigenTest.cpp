
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

TEST(EigenTest, test_memory_is_continuous_eigen_to_std_vector) {

	//eigen to std::vector
	std::vector<Eigen::Vector2f> x{ {1,2},{3,4} };

	std::vector<float> x_act(&x[0][0], &x[0][0] + 4);

	std::vector<float> x_exp{1,2,3,4 };

	EXPECT_EQ(x_act, x_exp);




}

TEST(EigenTest, test_memory_is_continuous_std_vector_to_eigen) {

	//eigen to std::vector

	std::vector<float> x{1,2,3,4 };

	Eigen::VectorXf x1=Eigen::VectorXf::Map( &x[0],x.size());

	Eigen::MatrixXf x2=Eigen::MatrixXf::Map( x.data(),x.size()/2,2);


	//std::cout << x1 << std::endl;

	Eigen::Vector2f x2c0 = x2.col(0);


	std::cout << x2.cols() << std::endl;

	std::cout << x2c0 << std::endl;


	std::cout << x2.col(0) << std::endl;
	std::cout << x2.col(1) << std::endl;

	std::cout << x2(0) << std::endl;
	std::cout << x2(1) << std::endl;

	std::cout << x2(2) << std::endl;
	std::cout << x2(3) << std::endl;

	std::cout << x2 << std::endl;
	

	std::vector<Eigen::Vector2f> x_exp{ {1,2},{3,4} };




	//to do
	//std::vector<Eigen::Vector2f> x_act(&x[0], &x[0]+ 4);
	//EXPECT_EQ(x_act, x_exp);

}

TEST(EigenTest, test_Matrix3Xf) {

	Eigen::Matrix3Xf m(3,3);
	m << 1, 2, 3, 4, 5, 6, 7, 8, 9;

	EXPECT_EQ(m.size(), 9);



}
