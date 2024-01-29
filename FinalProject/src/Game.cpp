//
//  Game.cpp
//

#include "Game.hpp"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Actor.hpp"
#include "Random.hpp"
#include "Enemy.hpp"
#include <iostream>

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mIsRunning(true)
,mTicksCount(0)
{
    
}

bool Game::Initialize()
{
   
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0)
    {
        SDL_Log("Failed to initializ SDL: %s", SDL_GetError());
        return false;
    }
    
    mWindow = SDL_CreateWindow(
                               "Tower Defense",      
                               250,               
                               100,               
                               1024,             
                               768,              
                               0                 
                               );
    
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    
   
    mRenderer = SDL_CreateRenderer(
                                   mWindow,
                                   -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
                                   );
    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("Failed to initializ SDL_image: %s", SDL_GetError());
        return false;
    }

    // init the text libraries
    if (TTF_Init() < 0) {
        printf("Text library TTF could not be Initialized correctly.\n");
    }

    // Load in the font 
    sans = TTF_OpenFont("fonts/abelregular.ttf", 36);
    if (!sans) { 
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
    
    Random::Init();
    
    LoadData();
    mTicksCount = SDL_GetTicks();
    return true;
}

void Game::LoadData() {
    mGrid = new Grid(this);
}

void Game::UnloadData()
{
    
    while (!mActors.empty())
    {
        delete mActors.back();
    }
    
    
    for (auto i : mTextures)
    {
        SDL_DestroyTexture(i.second);
    }
    mTextures.clear();
}

SDL_Texture* Game::LoadTexture(const char* fileName)
{
  
    SDL_Surface* surf = IMG_Load(fileName);
    
    if (!surf)
    {
        SDL_Log("Flaied to load  %s ", fileName);
        return nullptr;
    }
    
   
    SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, surf);
    SDL_FreeSurface(surf);
    if (!tex)
    {
        SDL_Log("%s surface failed to texture !", fileName);
        return nullptr;
    }
    return tex;
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
    SDL_Texture* tex = nullptr;
    
    auto iter = mTextures.find(fileName);
    if (iter != mTextures.end())
    {
        tex = iter->second;
    }
    else
    {
       
        SDL_Surface* surf = IMG_Load(fileName.c_str());
        if (!surf)
        {
            SDL_Log("Failed to load texture %s", fileName.c_str());
            return nullptr;
        }
        
       
        tex = SDL_CreateTextureFromSurface(mRenderer, surf);
        SDL_FreeSurface(surf);
        if (!tex)
        {
            SDL_Log("%s surface failed to texture", fileName.c_str());
            return nullptr;
        }
        
        mTextures.emplace(fileName.c_str(), tex);
    }
    return tex;
}

void Game::Shutdown()
{
    UnloadData();
    IMG_Quit();
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
        
        
    }
}

void Game::ProcessInput() {
    SDL_Event event;
    
  
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                mIsRunning = false;
                break;
            default:
                break;
        }
    }
    
    // Get keyboard state
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    
    // If pressing ESC，loop ends
    if (keyState[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }
    
    // If pressing B，building a tower
    if (keyState[SDL_SCANCODE_B])
    {
        mGrid->BuildTower();
    }
    
    //process mouse cloick
    int x, y;
    Uint32 buttons = SDL_GetMouseState(&x, &y);
    if (SDL_BUTTON(buttons) & SDL_BUTTON_LEFT)
    {
        mGrid->ProcessClick(x, y);
    }

    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->ProcessInput(keyState);
    }
    mUpdatingActors = false;
}

void Game::UpdateGame()
{
   
    // wait 16ms
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;
    
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }
    mTicksCount = SDL_GetTicks();
    
    // update all actors
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;
    
    // move pending actors to my actors
    for (auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();
    
    // add dead actor to deadActors
    std::vector<Actor*> deadActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == Actor::EDead)
        {
            deadActors.emplace_back(actor);
        }
    }
    
    //calculate the enymy number of arrival
    for (auto enemy : mEnemies) {

        if (enemy->getArrival())
           numArrivalEnemy++;
    }
    
    // delete actor from mActors
    for (auto actor : deadActors)
    {
        delete actor;
    }
    
}


void Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(mRenderer, 220, 220, 220, 255);
    SDL_RenderClear(mRenderer);
    

    for (auto sprite : mSprites)
    {
        sprite->Draw(mRenderer);
        
    }
  
   //dislpay the enemy number of arrival
    char msg[100];
    snprintf(msg,80, "Arrival Enemy: %d", numArrivalEnemy);
    int topY = 5;
    int leftX = 30;
    int height = 30;
    int width = 150;
    SDL_Rect stringRect = { leftX, topY, width, height };
    SDL_Color color = { 0, 0,255, 255 };
    drawText(msg, stringRect, color);
    
    // /dislpay the total enemy number 
    snprintf(msg, 80, "Enemy Number: %d", mGrid->GetNumEnemy());
    topY = 40;
    leftX = 30;
    height = 30;
    width = 150;
    stringRect = { leftX, topY, width, height };
    drawText(msg, stringRect, color);

    // /dislpay the total tower number 
    snprintf(msg, 80, "Tower Number: %d", mGrid->GetNumTower());
    topY = 75;
    leftX = 30;
    height = 30;
    width = 150;
    stringRect = { leftX, topY, width, height };
    drawText(msg, stringRect, color);

    //display who wins the game
    topY = 60;
    leftX = 380;
    height = 60;
    width = 200;
    stringRect = { leftX, topY, width, height };
    color = { 255, 0,0, 255 };

    if ( numArrivalEnemy > 0) {
        snprintf(msg, 80, "Game Over. AI Wins!");
        drawText(msg, stringRect, color);
        gameOver = true;
    }
    else if ( mGrid->GetNumEnemy() >= 50 && mEnemies.size() == 0) {
        snprintf(msg, 80, "Game Over. Player Wins!");
        drawText(msg, stringRect, color);
        gameOver = true;
    }
       
    SDL_RenderPresent(mRenderer);
   
    
}

void Game::drawText(const char* textToDraw, SDL_Rect messageRect, SDL_Color color)
{
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(sans, textToDraw, color); // TODO Make this print something other than m
    if (!surfaceMessage) { printf("TTF_OpenFont: %s\n", TTF_GetError()); }
    SDL_Texture* message = SDL_CreateTextureFromSurface(mRenderer, surfaceMessage);
    if (!message) { printf("Error 2\n"); }
    SDL_RenderCopy(mRenderer, message, NULL, &messageRect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
}

void Game::AddActor(Actor* actor)
{
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor)
{
    
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
      
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }
    
    
    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
     
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
        
    }
}

void Game::AddSprite(SpriteComponent* sprite)
{
    
    int myDrawOrder = sprite->GetDrawOrder();
    auto iter = mSprites.begin();
    for ( ;
         iter != mSprites.end();
         ++iter)
    {
        if (myDrawOrder < (*iter)->GetDrawOrder())
        {
            break;
        }
    }
    
    mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
    auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
    mSprites.erase(iter);
}

Enemy* Game::GetNearestEnemy(const Vector2& pos)
{
    Enemy* best = nullptr;
    
    if (mEnemies.size() > 0)
    {
        best = mEnemies[0];
        
        float bestDistSq = (pos - mEnemies[0]->GetPosition()).LengthSq();
        for (size_t i = 1; i < mEnemies.size(); i++)
        {
            float newDistSq = (pos - mEnemies[i]->GetPosition()).LengthSq();
            if (newDistSq < bestDistSq)
            {
                bestDistSq = newDistSq;
                best = mEnemies[i];
            }
        }
    }
    
    return best;
}
