//
//  MoveComponent.hpp
//  BasicPhysics
//


#ifndef MoveComponent_hpp
#define MoveComponent_hpp

#include "Component.hpp"

class MoveComponent : public Component
{
public:
    
    MoveComponent(class Actor* owner, int updateOrder = 10);
    
    void Update(float deltaTime) override;
    
    float GetAngularSpeed() const { return mAngularSpeed; }
    float GetForwardSpeed() const { return mForwardSpeed; }
    void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
    void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
private:
    
    float mAngularSpeed;
    float mForwardSpeed;
};


#endif /* MoveComponent_hpp */
