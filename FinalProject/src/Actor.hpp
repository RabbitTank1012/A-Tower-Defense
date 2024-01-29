//
//  Actor.hpp
// 


#ifndef Actor_hpp
#define Actor_hpp

#include <vector>
#include "Math.hpp"

class Actor
{
public:
    enum State
    {
        EActive,
        EPaused,
        EDead
    };
    
    Actor(class Game* game);
    virtual ~Actor();
    
    void Update(float deltaTime);
    void UpdateComponents(float deltaTime);
    virtual void UpdateActor(float deltaTime);
    
    // Getters/setters
    const Vector2& GetPosition() const { return mPosition; }
    void SetPosition(const Vector2& pos) { mPosition = pos; }
    float GetScale() const { return mScale; }
    void SetScale(float scale) { mScale = scale; }
    float GetRotation() const { return mRotation; }
    void SetRotation(float rotation) { mRotation = rotation; }
    
    Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }
    
    State GetState() const { return mState; }
    void SetState(State state) { mState = state; }
    
    class Game* GetGame() { return mGame; }
    
    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);
    
    void ProcessInput(const uint8_t* keyState);
    virtual void ActorInput(const uint8_t* keyState);
private:
    State mState;
    
    Vector2 mPosition;
    float mScale;
    float mRotation;
    
    std::vector<class Component*> mComponents;
    class Game* mGame;
};

#endif /* Actor_hpp */
