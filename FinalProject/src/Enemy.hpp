//
//  Enemy.hpp
//

#ifndef Enemy_hpp
#define Enemy_hpp
#include "Actor.hpp"

class Enemy : public Actor
{
public:
    Enemy(class Game* game);
    ~Enemy();
    void UpdateActor(float deltaTime) override;
    class CircleComponent* GetCircle() { return mCircle; }
   
    bool getArrival() {
        return arrived;
    }
private:
    class CircleComponent* mCircle;
    bool arrived = false;
};

#endif /* Enemy_hpp */
