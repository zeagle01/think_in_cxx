

#include "gtest/gtest.h"


#include <vector>
#include <string>
#include <iostream>
#include <memory>

struct Displacement{
    float x=0;
    float y=0;
};

struct Velocity{
    float x=0;
    float y=0;
};

std::string name;


enum Component{
    COMPONENT_NONE=0,
    COMPONENT_DISPLACEMENT=1<<0,
    COMPONENT_VELOCITY=1<<1,
    COMPONENT_APPEARANCE=1<<2,
};




struct World{
    const static int32_t MaxEntityNum=100;
    std::vector<unsigned int>  mask;
    std::vector<std::shared_ptr<Displacement>> X;
    std::vector<std::shared_ptr<Velocity>> V;
    std::vector<std::string> Name;
    World(){
	mask.assign(MaxEntityNum,COMPONENT_NONE);
	X.resize(MaxEntityNum);
	V.resize(MaxEntityNum);
	Name.resize(MaxEntityNum);
    }

};


int createEntity(World* world){
    for(int32_t i=0;i<world->MaxEntityNum;i++){
	if(world->mask[i]==COMPONENT_NONE){
	    return i;
	}
    }
    return world->MaxEntityNum;

}

void destroyEntity(World* world,int32_t entity){
    world->mask[entity]=COMPONENT_NONE;
}


void createMovable(World* world,float x,float y,float vx,float vy){
    unsigned int components=COMPONENT_DISPLACEMENT|COMPONENT_APPEARANCE;
    int32_t newEntity=createEntity(world);
    world->   mask[newEntity]=components;
    world->   X[newEntity]=std::make_shared<Displacement>();
    world->   X[newEntity]->x=x;
    world->   X[newEntity]->y=y;
    world->V[newEntity]=std::make_shared<Velocity>();
    world->V[newEntity]->x=vx;
    world->V[newEntity]->y=vy;
}

void createRendable(World* world,std::string name,float x,float y){

    int32_t newEntity=createEntity(world);
    world->   X[newEntity]=std::make_shared<Displacement>();
    world->   X[newEntity]->x=x;
    world->   X[newEntity]->y=y;
    world-> Name[newEntity]=name;

}



void moveSystem(World* world){
    unsigned int movementMask=COMPONENT_DISPLACEMENT|COMPONENT_VELOCITY;
    for(int32_t i=0;i<world->mask.size();i++){
	if((world->mask[i]&movementMask)==movementMask){
	    auto  x=world->X[i];
	    auto v=world->V[i];
	    v->y=-10;
	    x->x+=v->x;
	    x->y+=v->y;



	}
    }
}


void renderSystem(World* world){
    unsigned int renderMask=COMPONENT_DISPLACEMENT|COMPONENT_APPEARANCE;
    for(int32_t i=0;i<world->mask.size();i++){
	if((world->mask[i]&renderMask)==renderMask){
	    auto x=world->X[i];
	    std::string name=world->Name[i];
	    std::cout<<name<<" "<<"at "<<x->x<<" "<<x->y<<std::endl;
	}
    }
}


TEST(ECSSymstemTest,test_one){
    World world;
    createMovable(&world,0,0,1,0);
    createMovable(&world,1,0,1,0);
    createRendable(&world,"a",2,0);
    createRendable(&world,"b",3,0);
    createRendable(&world,"c",4,0);
    moveSystem(&world);
    renderSystem(&world);
}
