


#include "gtest/gtest.h"



class ActLikeValue{
	friend  void swap(ActLikeValue& o0,ActLikeValue& o1);
    public:
	
	ActLikeValue(int32_t value){
	    this->value=new int32_t;
	    *(this->value)=value;
	}

	ActLikeValue(const ActLikeValue& other){
	    this->value=new int32_t;
	    //*(this->value)=*(other.value);
	    *(this->value)=other.getValue();
	}

	// if a class defines a swap,then copy and swap(CAS) can come in handy
	ActLikeValue& operator=(ActLikeValue other){
		swap(*this,other);
	    return *this;
	}

//	ActLikeValue& operator=(const ActLikeValue& other){
//	    *(this->value)=other.getValue();
//	    return *this;
//	}




	int32_t getValue() const {
	    return *value;
	}
	void setValue(int32_t value){
	    *(this->value)=value;
	}

    private:
	int32_t* value;
};

void swap(ActLikeValue& o0,ActLikeValue& o1){
	using std::swap;
	swap(o0.value,o1.value);
//	int32_t * tmp=o0.value;
//	o0.value=o1.value;
//	o1.value=tmp;
}

TEST(ActLikeValueTest,test_use){


    ActLikeValue a0(0);

    EXPECT_EQ(a0.getValue(),0);

    //copy constructor
    ActLikeValue a1(a0);
    
    EXPECT_EQ(a1.getValue(),0);

    a1.setValue(2);

    EXPECT_EQ(a1.getValue(),2);
    EXPECT_EQ(a0.getValue(),0);


    // operator =
    ActLikeValue a2(3);

    EXPECT_EQ(a2.getValue(),3);

    a2=a0;


    EXPECT_EQ(a2.getValue(),0);
    EXPECT_EQ(a0.getValue(),0);

}

TEST(ActLikeValueTest,test_swap){
	ActLikeValue v0(0);
	ActLikeValue v1(1);
	swap(v0,v1);
	EXPECT_EQ(v0.getValue(),1);
	EXPECT_EQ(v1.getValue(),0);
}



