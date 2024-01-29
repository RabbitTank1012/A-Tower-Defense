//
//  Grid.cpp
//  GameAI
//


#include "Grid.hpp"
#include "Tile.hpp"
#include "Tower.hpp"
#include "Enemy.hpp"
#include <algorithm>
#include <iostream>

Grid::Grid(class Game* game)
:Actor(game)
,mSelectedTile(nullptr)
{
    // 7 rows, 16 columns
    mTiles.resize(kNumRows);
    for (size_t i = 0; i < mTiles.size(); i++)
    {
        mTiles[i].resize(kNumCols);
    }
    
    // create tile
    for (size_t i = 0; i < kNumRows; i++)
    {
        for (size_t j = 0; j < kNumCols; j++)
        {
            mTiles[i][j] = new Tile(GetGame());
            mTiles[i][j]->SetPosition(Vector2(kTileSize/2.0f + j * kTileSize, kStartY + i * kTileSize));
        }
       
    }
    
    // set start and destination
    GetStartTile()->SetTileState(Tile::EStart);
    GetEndTile()->SetTileState(Tile::EBase);
    
    // establish an adjacency list for every tile
    for (size_t i = 0; i < kNumRows; i++)
    {
        for (size_t j = 0; j < kNumCols; j++)
        {
            if (i > 0)
            {
                mTiles[i][j]->mAdjacent.push_back(mTiles[i-1][j]);
            }
            if (i < kNumRows - 1)
            {
                mTiles[i][j]->mAdjacent.push_back(mTiles[i+1][j]);
            }
            if (j > 0)
            {
                mTiles[i][j]->mAdjacent.push_back(mTiles[i][j-1]);
            }
            if (j < kNumCols - 1)
            {
                mTiles[i][j]->mAdjacent.push_back(mTiles[i][j+1]);
            }
        }
    }
    
    // reverse pathfinding
    FindPath(GetEndTile(), GetStartTile());
    UpdatePathTiles(GetStartTile());
    
    mNextEnemy = kEnemyTime;
}

void Grid::SelectTile(size_t row, size_t col)
{
    Tile::TileState tstate = mTiles[row][col]->GetTileState();
    if (tstate != Tile::EStart && tstate != Tile::EBase)
    {
        if (mSelectedTile)
        {
            mSelectedTile->ToggleSelect();
        }
        mSelectedTile = mTiles[row][col];
        mSelectedTile->ToggleSelect();
    }
}

void Grid::ProcessClick(int x, int y)
{
    y -= static_cast<int>(kStartY - kTileSize / 2);
    if (y >= 0)
    {
        x /= static_cast<int>(kTileSize);
        y /= static_cast<int>(kTileSize);
        if (x >= 0 && x < static_cast<int>(kNumCols) && y >= 0 && y < static_cast<int>(kNumRows))
        {
            SelectTile(y, x);
        }
    }
}

// implement of A* algorithm
bool Grid::FindPath(Tile* start, Tile* goal)
{
    for (size_t i = 0; i < kNumRows; i++)
    {
        for (size_t j = 0; j < kNumCols; j++)
        {
            mTiles[i][j]->g = 0.0f;
            mTiles[i][j]->mInOpenSet = false;
            mTiles[i][j]->mInClosedSet = false;
        }
    }
    
    std::vector<Tile*> openSet;
    Tile* current = start;
    current->mInClosedSet = true;
    
    do
    {
        //  Add neighbor node to an open set
        for (Tile* neighbor : current->mAdjacent)
        {
            if (neighbor->mBlocked)
            {
                continue;
            }
            
          
            if (!neighbor->mInClosedSet)
            {
                if (!neighbor->mInOpenSet)
                {
                    // Not in the open set, so set parent
                    neighbor->mParent = current;
                    neighbor->h = (neighbor->GetPosition() - goal->GetPosition()).Length();
                    // g(x) is the parent's g plus cost of traversing edge
                    neighbor->g = current->g + kTileSize;
                    neighbor->f = neighbor->g + neighbor->h;
                    openSet.emplace_back(neighbor);
                    neighbor->mInOpenSet = true;
                }
                else
                {
                    float newG = current->g + kTileSize;
                    if (newG < neighbor->g)
                    {
                        neighbor->mParent = current;
                        neighbor->g = newG;
                        neighbor->f = neighbor->g + neighbor->h;
                    }
                }
            }
        }
        
        if (openSet.empty())
        {
            break;
        }
        
        // find the node with the lowest cost in openset
        auto iter = std::min_element(openSet.begin(), openSet.end(),
                                     [](Tile* a, Tile* b) {
                                         return a->f < b->f;
                                     });
        // set current node and move it from openset to closedset
        current = *iter;
        openSet.erase(iter);
        current->mInOpenSet = false;
        current->mInClosedSet = true;
    }
    while (current != goal);
    
    //whether a path is found
    return (current == goal) ? true : false;
}

void Grid::UpdatePathTiles(class Tile* start)
{
    // set to default values, except for starting and ending node
    for (size_t i = 0; i < kNumRows; i++)
    {
        for (size_t j = 0; j < kNumCols; j++)
        {
            if (!(i == kNumRows / 2 && j == 0) && !(i == kNumRows / 2 && j == kNumCols - 1))
            {
                mTiles[i][j]->SetTileState(Tile::EDefault);
            }
        }
    }
    
    //set path node
    Tile* t = start->mParent;
    while (t != GetEndTile())
    {
        t->SetTileState(Tile::EPath);
        t = t->mParent;
    }
}

void Grid::BuildTower()
{
    if (numTower < 5)
    {
        if (mSelectedTile && !mSelectedTile->mBlocked)
        {
            mSelectedTile->mBlocked = true;
            if (FindPath(GetEndTile(), GetStartTile()))
            {
                Tower* t = new Tower(GetGame());
                t->SetPosition(mSelectedTile->GetPosition());
                numTower++;
            }
            else
            {
                // defensive tower is not allowed to be built if it blocks the path
                mSelectedTile->mBlocked = false;
                FindPath(GetEndTile(), GetStartTile());
            }
            UpdatePathTiles(GetStartTile());
        }
    }
}

Tile* Grid::GetStartTile()
{
    return mTiles[3][0];
}

Tile* Grid::GetEndTile()
{
    return mTiles[3][15];
}

void Grid::UpdateActor(float deltaTime)
{
    Actor::UpdateActor(deltaTime);
    
    // time to produce a new enemy
    mNextEnemy -= deltaTime;
    if (mNextEnemy <= 0.0f)
    {
        Game* game = GetGame();
       
        if (!game->GetGameOver()&& numEnemy < 50) {
            new Enemy(GetGame());
            mNextEnemy += kEnemyTime;
            numEnemy = numEnemy + 1;
        }
    }

  
}
