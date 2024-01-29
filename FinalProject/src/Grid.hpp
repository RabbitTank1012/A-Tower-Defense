//
//  Grid.hpp
//  
//

#ifndef Grid_hpp
#define Grid_hpp

#include "Actor.hpp"
#include <vector>
#include "Game.hpp"

class Grid : public Actor
{
public:
    Grid(class Game* game);
    
    // process mouse click
    void ProcessClick(int x, int y);
    
    // use A* pathfinding algorithm
    bool FindPath(class Tile* start, class Tile* goal);
    
    // build tower
    void BuildTower();
    
    // get the start and end of the game
    class Tile* GetStartTile();
    class Tile* GetEndTile();

    void UpdateActor(float deltaTime) override;

    int GetNumEnemy() {
        return  numEnemy;
    }

    int GetNumTower() {
        return  numTower;
    }

private:
    
    void SelectTile(size_t row, size_t col);
    void UpdatePathTiles(class Tile* start);
    
    // the selected tile
    class Tile* mSelectedTile;
     // all tiles
    std::vector<std::vector<class Tile*>> mTiles;
    float mNextEnemy;
    
    // the number of rows and columns in the grid
    const size_t kNumRows = 7;
    const size_t kNumCols = 16;
    
    const float kStartY = 192.0f;
    const float kTileSize = 64.0f;
    
    // The time between two enemies
    const float kEnemyTime = 1.2f;

    int numEnemy =0;
    int numTower = 0;
         
};


#endif /* Grid_hpp */
