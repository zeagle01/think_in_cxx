


#include "gtest/gtest.h"
#include <functional>

using namespace testing;

namespace event_dispatch 
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

	class Event_A:public Event_Base,public identity<Event_A>
	{
	public:
		virtual Event get_event_type();
	};

	class Event_B:public Event_Base,public identity<Event_B>
	{
	public:
		virtual Event get_event_type();
	};

	class Event_C:public Event_Base,public identity<Event_C>
	{
	public:
		virtual Event get_event_type();
	};

	
	template<Event ev,typename T>
	struct event_traits:public identity<T>
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
