


#include "gmock/gmock.h"
#include <functional>
#include <memory>

using namespace testing;

namespace event_dispatch 
{

	namespace _1
	{
		enum class Event
		{
			None,
			Event_A,
			Event_B,
			Event_C,
			Size

		};

		class Event_Base
		{
		public:
			virtual Event get_event_type() = 0;
		};

		template<typename T>
		struct identity
		{
			using type = T;
		};

		class Event_A :public Event_Base, public identity<Event_A>
		{
		public:
			virtual Event get_event_type();
		};

		class Event_B :public Event_Base, public identity<Event_B>
		{
		public:
			virtual Event get_event_type();
		};

		class Event_C :public Event_Base, public identity<Event_C>
		{
		public:
			virtual Event get_event_type();
		};


		template<Event ev, typename T>
		struct event_traits :public identity<T>
		{
			static constexpr Event enum_type = ev;
		};

		template<Event ev>
		struct e_2_t :public event_traits<ev, void>
		{

		};

		template<typename T>
		struct t_2_e :public event_traits<Event::None, T>
		{

		};



#define Configure_Enum_Map(x) \
	template<> \
	struct e_2_t<Event::x> :public event_traits<Event::x, x> {};\
	template<> \
	struct t_2_e<x> :public event_traits<Event::x, x> {};




		Configure_Enum_Map(Event_A)
			Configure_Enum_Map(Event_B)
			Configure_Enum_Map(Event_C)


			Event Event_A::get_event_type()
		{
			return t_2_e<type>::enum_type;
		}

		Event Event_B::get_event_type()
		{
			return t_2_e<type>::enum_type;
		}

		Event Event_C::get_event_type()
		{
			return t_2_e<type>::enum_type;
		}



#define Configure_Fn(m,en,fn) \
		m[en]= [](Event_Base& e) \
		{ \
			e_2_t<en>::type& e_actual = (e_2_t<Event::Event_A>::type&) e; \
			fn(e_actual); \
		} \



		TEST(Configure_Type, type_is_right)
		{
			//given a event
			std::unique_ptr<Event_Base> e = std::make_unique< Event_A>();

			//configure the handling function
			std::map<Event, std::function<void(Event_Base&)>> m;
			Configure_Fn(m, Event::Event_A, [](auto& e) { printf("e is handling\n"); });

			//calling the handling function
			m[e->get_event_type()](*e);
		}

	}


	namespace _2
	{

		template<typename...P>
		struct type_list {};



		template<typename L>
		struct front_imp;

		template<typename H, typename ...P>
		struct front_imp<type_list<H, P...>>
		{
			using type = H;
		};

		template<typename L>
		using front = typename front_imp<L>::type;


		template<typename L>
		struct pop_front_imp;

		template<typename H, typename ...P>
		struct pop_front_imp<type_list<H, P...>>
		{
			using type = type_list<P...>;
		};

		template<typename L>
		using pop_front= typename pop_front_imp<L>::type;

		template<typename L>
		struct is_empty_imp
		{
			static constexpr bool value = false;
		};

		template<>
		struct is_empty_imp<type_list<>>
		{
			static constexpr bool value = true;
		};


		template<typename L>
		inline static constexpr  bool is_empty = is_empty_imp<L>::value;



		////////////////////




		class Event
		{
		public:
			virtual ~Event() {}
		};

		class Event_A :public Event
		{
		public:
			std::string spercific_with_event_a()
			{
				return "A";
			}
		};

		class Event_B :public Event
		{
		public:
			std::string spercific_with_event_b() {
				return "B";
			}
		};


		class Event_Handler
		{
		public:
			std::string operator()( Event_A e) { return "handle " + e.spercific_with_event_a(); }
			std::string operator()( Event_B e) { return "handle " + e.spercific_with_event_b(); }
			std::string operator()( Event e) { return "error"; }

		};




		template<typename Base_T, typename Derived_List, typename Fn, typename R = void>
		struct Static_Dispatcher
		{

			static R disptch(Base_T* x,Fn fn)
			{
				using current_t = front<Derived_List>;

				if (auto p = dynamic_cast<current_t*>(x))
				{

					return fn(*p);
				}
				else
				{
					using poped_front_list = pop_front<Derived_List>;

					if constexpr (!is_empty<poped_front_list>)
					{
						return Static_Dispatcher<Base_T, poped_front_list, Fn, R>::disptch(x, fn);
					}

					return R{  };
				}
			}
			
		};




		TEST(Event_Dispatch_With_Static_Dispatcher,use)
		{
			using Events = type_list<Event_B, Event_A>;

			std::unique_ptr<Event> e = std::make_unique<Event_A>();
			Event_Handler eh;

			auto act = Static_Dispatcher<Event, Events, Event_Handler, std::string>::disptch(e.get(), eh);

			EXPECT_THAT(act, Eq("handle A"));
		}


	}


}
