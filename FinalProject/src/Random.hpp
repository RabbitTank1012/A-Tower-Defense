//
//  Random.hpp
//  BasicPhysics
//


#ifndef Random_hpp
#define Random_hpp

#include <random>
#include "Math.hpp"

class Random
{
public:
    static void Init();
    static void Seed(unsigned int seed);
    static float GetFloat();
    static float GetFloatRange(float min, float max);
    static int GetIntRange(int min, int max);
    static Vector2 GetVector(const Vector2& min, const Vector2& max);
   // static Vector3 GetVector(const Vector3& min, const Vector3& max);
private:
    static std::mt19937 sGenerator;
};


#endif /* Random_hpp */
