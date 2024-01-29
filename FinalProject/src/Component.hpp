//
//  Component.hpp
//


#ifndef Component_hpp
#define Component_hpp
#include <cstdint>

#include "Actor.hpp"

class Component
{
public:
   
    Component(class Actor* owner, int updateOrder = 100);
    virtual ~Component();
    
    virtual void Update(float deltaTime);
    virtual void ProcessInput(const uint8_t* keyState) {}
    int GetUpdateOrder() const { return mUpdateOrder; }

protected:
    class Actor* mOwner;
    int mUpdateOrder;
};

#endif /* Component_hpp */
