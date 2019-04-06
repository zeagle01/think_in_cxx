


#include "gtest/gtest.h"



class ActLikeValue{
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

	ActLikeValue& operator=(const ActLikeValue& other){
	    *(this->value)=other.getValue();
	    return *this;
	}




	int32_t getValue() const {
	    return *value;
	}
	void setValue(int32_t value){
	    *(this->value)=value;
	}

    private:
	int32_t* value;
};



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

    a2.setValue(2);

    EXPECT_EQ(a2.getValue(),2);
    EXPECT_EQ(a0.getValue(),0);




}
