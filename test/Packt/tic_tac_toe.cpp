

#include "gmock/gmock.h"

#include <vector>
#include <algorithm>
#include <numeric> 

using namespace testing;
using namespace std::placeholders;

namespace Packt_C1
{

	struct Is_Odd
	{
		bool operator()(const int i) const
		{
			return i % 2 == 1;
		}
	};

	struct Is_Even
	{
		bool operator()(const int i) const
		{
			return i % 2 == 0;
		}
	};

	template <typename UnitaryOp>
	std::vector<int> filter(const std::vector<int> in, UnitaryOp op)
	{
		std::vector<int> ret;

		std::copy_if(in.begin(), in.end(), back_inserter(ret), op);
		return ret;
	}

	int sum(std::vector<int>& in)
	{
		int sum = std::accumulate(in.begin(), in.end(), 0);
		return sum;
	}

	TEST(C01, composable_sum)
	{
		Is_Even is_even;
		Is_Odd is_odd;
		std::vector<int> in{ 0, 1, 2, 3, 4 };

		int s = sum(in);
		int even_sum = sum(filter(in, is_even));
		int odd_sum = sum(filter(in, is_odd));

		int exp = 10;
		int exp_even_sum = 6;
		int exp_odd_sum = 4;
		EXPECT_THAT(s, Eq(exp));
		EXPECT_THAT(even_sum, Eq(exp_even_sum));
		EXPECT_THAT(odd_sum, Eq(exp_odd_sum));
	}




	////////////////TIC_TOK_TOE///////////////////////////
	auto any_of_collection = [](const auto& collection, auto predacate)
	{
		return std::any_of(collection.begin(), collection.end(), predacate);
	};

	auto all_of_collection = [](const auto& collection, auto predacate)
	{
		return std::all_of(collection.begin(), collection.end(), predacate);
	};

	template<typename DstCollectionT>
	auto transform_of_collection = [](const auto& collection, auto fn)
	{
		DstCollectionT dst;
		dst.reserve(collection.size());
		std::transform(collection.begin(), collection.end(), back_inserter(dst), fn);
		return dst;
	};


	auto is_char = [](const auto& c, const auto& v) {return c == v; };

	auto is_char_a = std::bind(is_char, _1, 'a');
	auto is_char_x = std::bind(is_char, _1, 'x');

	auto to_char_a = [](const auto& c) {return 'a'; };



	using Line = std::vector<char>;
	using Board = std::vector<Line>;
	using Coodinate = std::pair<int, int>;


	Board b0{
	{'x','x','x'},
	{'o','o','o'},
	{'x','o','x'}
	};


	template<typename F0,typename F1>
	auto compose(F0 f0, F1 f1)
	{
		return [=](const auto& p) {return f0(f1(p)); };
	}

	template<typename F0,typename F1,typename F2>
	auto compose(F0 f0, F1 f1,F2 f2)
	{
		return [=](const auto& p) {return f0(f1(f2(p))); };
	}

	auto access_at_coodinate = [](const auto& board, const auto& c)
	{
		return board[c.first][c.second];
	};

	auto access_at_coodinates = [](const auto& board, const auto& coodinates)
	{
		auto bind_board = std::bind(access_at_coodinate, board, _1);
		return transform_of_collection<Line>(coodinates, bind_board);
	};


	auto to_range = [](const auto& size)
	{
		std::vector<int> range(size);
		std::iota(range.begin(), range.end(), 0);
		return range;
	};

	auto get_coodinate = [](const auto& r, const auto& c)
	{
		return std::make_pair(r, c);
	};

	auto get_coodinates = [](const auto& size, auto fn)
	{
		auto range = to_range(size);
		return transform_of_collection<std::vector<Coodinate>>(range, fn);
	};

	auto row_coodinates = [](const auto& board, const auto& row)
	{
		return get_coodinates(board.size(), std::bind(get_coodinate, row, _1));
	};

	auto col_coodinates = [](const auto& board, const auto& col)
	{
		return get_coodinates(board.size(), std::bind(get_coodinate, _1, col));
	};

	auto diagonal_coodinates = [](const auto& board)
	{
		return get_coodinates(board.size(), std::bind(get_coodinate, _1, _1));
	};

	auto off_diagonal_coodinates = [](const auto& board)
	{
		int n = static_cast<int>(board.size());
		auto fn = [n](const auto& index) {
			return get_coodinate(index, n - 1 - index);
		};

		return get_coodinates(board.size(), fn);
	};


	auto get_align = [](const auto& board,auto coodinates_fn,const auto& index)
	{
		return access_at_coodinates(board, coodinates_fn(board, index));
	};

	auto all_aligns = [](const auto& size,auto coodinates_fn, const auto& board)
	{
		auto fn = std::bind(get_align, board, coodinates_fn, _1);
		auto range = to_range(size);
		return transform_of_collection<std::vector<Line>>(range, fn);
	};


	auto all_rows = [](const auto& board)
	{
		return all_aligns(board.size(), row_coodinates, board);
	};

	auto all_cols = [](const auto& board)
	{
		return all_aligns(board.size(), col_coodinates, board);
	};

	auto all_diagonals = [](const auto& board)
	{
		auto fn = [](const auto& board, const auto& index) {return diagonal_coodinates(board); };
		return all_aligns(1, fn, board);
	};

	auto all_off_diagonals = [](const auto& board)
	{
		auto fn = [](const auto& board, const auto& index) {return off_diagonal_coodinates(board); };
		return all_aligns(1, fn, board);
	};

	////////////////////////////////

	TEST(TIC_TOK_TOE, all_of_collection)
	{


		std::vector<char> in{ 'a','b','c' };

		bool act = all_of_collection(in, is_char_a);

		EXPECT_THAT(act, Eq(false));

	}


	TEST(TIC_TOK_TOE, any_of_collection)
	{


		std::vector<char> in{ 'a','b','c' };

		bool act = any_of_collection(in, is_char_a);

		EXPECT_THAT(act, Eq(true));

	}


	TEST(TIC_TOK_TOE, line_fill_with_x)
	{
		std::vector<char> line{ 'x','x','x' };
		auto act = all_of_collection(line, is_char_x);
		EXPECT_THAT(act, Eq(true));
	}


	TEST(TIC_TOK_TOE, transform_collection)
	{
		std::vector<char> in{ 'a','b','c' };
		std::vector<char> exp{ 'a','a','a' };

		auto act = transform_of_collection<std::vector<char>>(in, to_char_a);
		EXPECT_THAT(act, Eq(exp));

	}

	TEST(TIC_TOK_TOE, compose_to_aaa_then_all_of)
	{
		std::vector<char> in{ 'a','b','c' };

		auto f0 = [](const auto& in) {
			return all_of_collection(in, is_char_a);
		};//todo: use bind

		auto f1 = std::bind(transform_of_collection<std::vector<char>>, _1, to_char_a);

		auto act = compose(f0, f1)(in);

		EXPECT_THAT(act, Eq(true));
	}

	TEST(TIC_TOK_TOE, transform_collection_to_string)
	{
		std::vector<char> in{ 'a','b','c' };
		std::string exp{ "aaa" };

		auto act = transform_of_collection<std::string>(in, to_char_a);
		EXPECT_THAT(act, Eq(exp));

	}

	TEST(TIC_TOK_TOE, transform_collection_to_std_vector_int)
	{
		std::vector<char> in{ 'a','b','c' };
		std::vector<int> exp{ 0,1,2 };

		auto char_to_int = [](const auto& c) {return int(c - 'a'); };

		auto act = transform_of_collection<std::vector<int>>(in, char_to_int);
		EXPECT_THAT(act, Eq(exp));

	}

	TEST(TIC_TOK_TOE, access_board)
	{
		Coodinate c = std::make_pair<int, int>(0, 0);
		auto act = access_at_coodinate(b0, c);
		EXPECT_THAT(act, Eq('x'));
	}

	TEST(TIC_TOK_TOE, access_board_with_multiple_coodinates)
	{
		std::vector<Coodinate> coodinates{
			{0,0},
			{1,1},
			{2,2}
		};
		auto act = access_at_coodinates(b0, coodinates);

		Line exp{ 'x','o','x' };
		EXPECT_THAT(act, Eq(exp));
	}

	TEST(TIC_TOK_TOE, row_coodinates)
	{
		auto act = row_coodinates(b0, 0);

		std::vector<Coodinate> exp{
			{0,0},
			{0,1},
			{0,2}
		};

		EXPECT_THAT(act, Eq(exp));
	}


	TEST(TIC_TOK_TOE, col_coodinates)
	{
		auto act = col_coodinates(b0, 0);

		std::vector<Coodinate> exp{
			{0,0},
			{1,0},
			{2,0}
		};

		EXPECT_THAT(act, Eq(exp));
	}


	TEST(TIC_TOK_TOE, diag_coodinates)
	{
		auto act = diagonal_coodinates(b0);

		std::vector<Coodinate> exp{
			{0,0},
			{1,1},
			{2,2}
		};

		EXPECT_THAT(act, Eq(exp));
	}

	TEST(TIC_TOK_TOE, off_diag_coodinates)
	{
		auto act = off_diagonal_coodinates(b0);

		std::vector<Coodinate> exp{
			{0,2},
			{1,1},
			{2,0}
		};

		EXPECT_THAT(act, Eq(exp));
	}


	TEST(TIC_TOK_TOE, get_all_row)
	{
		auto act = all_rows(b0);

		std::vector<Line> exp{
			{'x','x','x'},
			{'o','o','o'},
			{'x','o','x'}
		};

		EXPECT_THAT(act, Eq(exp));
	}

	TEST(TIC_TOK_TOE, get_all_col)
	{
		auto act = all_cols(b0);

		std::vector<Line> exp{
			{'x','o','x'},
			{'x','o','o'},
			{'x','o','x'}

		};

		EXPECT_THAT(act, Eq(exp));
	}

	TEST(TIC_TOK_TOE, get_all_diagonals)
	{
		auto act = all_diagonals(b0);

		std::vector<Line> exp{
			{'x','o','x'},
		};
		EXPECT_THAT(act, Eq(exp));
	}

	TEST(TIC_TOK_TOE, get_all_off_diagonals)
	{
		auto act = all_off_diagonals(b0);

		std::vector<Line> exp{
			{'x','o','x'},
		};
		EXPECT_THAT(act, Eq(exp));
	}





	///////////////////////////////std_algorithms
	TEST(std_algorithms, all_of)
	{

		std::vector<char> in{ 'a','b','c' };
		bool act = std::all_of(in.begin(), in.end(), is_char_a);
		EXPECT_THAT(act, Eq(false));

	}

	TEST(std_algorithms, transform)
	{

		std::vector<char> in{ 'a','b','c' };
		std::vector<char> act;
		act.reserve(in.size());
		std::transform(in.begin(), in.end(), back_inserter(act), to_char_a);
		auto exp = std::vector<char>{ 'a','a','a' };
		EXPECT_THAT(act, Eq(exp));

	}




} // namespace Packt_C1
