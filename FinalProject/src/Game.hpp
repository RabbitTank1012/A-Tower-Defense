//
//  Game.hpp
//

#ifndef Game_hpp
#define Game_hpp
#include <vector>
#include <string>
#include <unordered_map>
#include "SDL.h"
#include "Math.hpp"
#include "SpriteComponent.hpp"
#include "Enemy.hpp"
#include "Grid.hpp"
#include "SDL_ttf.h"

// Game class
class Game
{
public:
    Game();
    
    bool Initialize();
    void RunLoop();
    void Shutdown();
    
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);
    
    void AddSprite(SpriteComponent* sprite);
    void RemoveSprite(SpriteComponent* sprite);
    
    SDL_Texture* GetTexture(const std::string& fileName);
    
    class Grid* GetGrid() { return mGrid; }

    class Enemy* GetNearestEnemy(const Vector2& pos);

    std::vector<class Enemy*>& GetEnemies() { return mEnemies; }
   
    bool GetGameOver() {
        return gameOver;
    }
private:
    void LoadData();
    void UnloadData();
    SDL_Texture* LoadTexture(const char* fileName);
    
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void drawText(const char* textToDraw, SDL_Rect messageRect, SDL_Color color);
    
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    TTF_Font* sans;

    bool mIsRunning;
    Uint32 mTicksCount;
    
    std::vector<class Actor*> mActors;
    std::vector<class SpriteComponent*> mSprites;
    std::vector<class Actor*> mPendingActors;
    std::vector<class Enemy*> mEnemies;

    bool mUpdatingActors;
    
    // store loaded textures
    std::unordered_map<std::string, SDL_Texture*> mTextures;
    
    class Grid* mGrid;
    float mNextEnemy;
    int numArrivalEnemy = 0;
    bool gameOver = false;
};

#endif /* Game_hpp */

