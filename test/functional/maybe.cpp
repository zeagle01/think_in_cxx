


#include "gmock/gmock.h"
#include <memory>



using namespace testing;


namespace maybe
{


    template<typename T>
	class Maybe;

	template<typename T>
	Maybe<T> maybe(T* context);


    template<typename T>
	class Maybe
	{
	public:
		Maybe(T* context) :context(context) {}

		template<typename acceccorFn>
		auto with(acceccorFn fn)
		{
			return context != nullptr ? maybe(fn(context)) : nullptr;

			//not works use if
//			if (context)
//			{
//				return  maybe(fn(context));
//			}
//			else
//			{
//				return maybe(nullptr);
//			}
		}




		template<typename Fn>
		auto act(Fn fn)
		{
			if (context)
			{
				 fn(context);
			}
			return *this;
		}



	private:
		T* context;
	};

	template<typename T>
	Maybe<T> maybe(T* context)
	{
		return Maybe<T>(context);
	}





	struct Pos
	{
		int x;
	};

    struct Person
    {
		Pos* pos;
    };



	class Maybe_Test:public Test
	{
	protected:
		std::string act;
		std::shared_ptr<Person> m_person;
		std::shared_ptr<Pos> m_pos;
	};


	TEST_F(Maybe_Test, chain_broke)
	{

		m_person = std::make_shared<Person>();


		int out = -1;

		int exp = out;

		auto r = maybe(m_person.get())
			.with([](auto x) {return x->pos; })
			.act([&out](auto x) {out = x->x; });


		EXPECT_THAT(out, Eq(exp));


	}

	TEST_F(Maybe_Test, chain_complete)
	{
		m_person = std::make_shared<Person>();

		m_pos = std::make_shared<Pos>();

		int exp = 100;

		m_pos->x = exp;

		m_person->pos = m_pos . get();

		int out = -1;

		auto r = maybe(m_person.get())
			.with([](auto x) {return x->pos; })
			.act([&out](auto x) {out = x->x; });


		EXPECT_THAT(out, Eq(exp));


	}

    


}
