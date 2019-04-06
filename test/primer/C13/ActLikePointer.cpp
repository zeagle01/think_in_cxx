
#include "gtest/gtest.h"
#include <iostream>



class ActLikePointer{
    public:
	static int32_t obj_count;
	static int32_t data_count;
	ActLikePointer(){
		id=obj_count;
		data_id=data_count;
		data=new int32_t[10];
		ref_count=new int32_t;
		*ref_count=1;
	    std::cout<<"in "<<id<<"'s constructor"<<std::endl;
		std::cout << "---- data " << data_id << " is created" << std::endl;

		obj_count++;
		data_count++;
	}


	ActLikePointer(const ActLikePointer& other){
		id=obj_count;
		copy_data(other);

	}

	ActLikePointer& operator =(const ActLikePointer& other){
		try_free();
		copy_data(other);
		return *this;
	}
	
	~ActLikePointer(){
		std::cout << "in " << id << "'s destructor" << std::endl;
		try_free();

	}
    private:
	  void copy_data(const ActLikePointer &other)
	  {
		  data = other.data;
		  ref_count = other.ref_count;
		  (*ref_count)++;
	  }

	  void try_free()
	  {
		  (*ref_count)--;
		  if (*ref_count == 0)
		  {
			  std::cout << "---- data " << data_id << " is destroyed" << std::endl;
			  delete[] data;
			  delete ref_count;
			  data = nullptr;
			  ref_count = nullptr;
		  }
	  }

	  int32_t id;
	  int32_t data_id;
	  int32_t *data;
	  int32_t *ref_count;

};
int32_t ActLikePointer::obj_count=0;
int32_t ActLikePointer::data_count=0;

TEST( ActLikePointerTest,test_copy_constructor){
    ActLikePointer p0;
	{
		ActLikePointer p1 = p0;
	}
}

TEST( ActLikePointerTest,test_operator_equal){
    ActLikePointer p0;
	{
		ActLikePointer p1 ;
		p1=p0;
	}
}













