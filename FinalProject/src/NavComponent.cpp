//
//  NavComponent.cpp
//  GameAI
//


#include "NavComponent.hpp"
#include "Tile.hpp"
#include <iostream>

NavComponent::NavComponent(class Actor* owner, int updateOrder)
:MoveComponent(owner, updateOrder)
,mNextNode(nullptr)
{
    
}

void NavComponent::Update(float deltaTime)
{
    if (mNextNode)
    {
        
        Vector2 diff = mOwner->GetPosition() - mNextNode->GetPosition();
        if (Math::NearZero(diff.Length(), 2.0f))
        {
          //  std::cout << "diff.Length():" << diff.Length()<<std::endl;
            mNextNode = mNextNode->GetParent();
            TurnTo(mNextNode->GetPosition());
          //  std::cout << "mNextNode->GetPosition():" << mNextNode->GetPosition().x <<"," << mNextNode->GetPosition().y << std::endl;
        }
    }
    
    MoveComponent::Update(deltaTime);
}

void NavComponent::StartPath(const Tile* start)
{
    mNextNode = start->GetParent();
    TurnTo(mNextNode->GetPosition());
}

void NavComponent::TurnTo(const Vector2& pos)
{
    
    Vector2 dir = pos - mOwner->GetPosition();
    float angle = Math::Atan2(-dir.y, dir.x);
    mOwner->SetRotation(angle);
  //  std::cout << "angle:" << angle <<  std::endl;
}
