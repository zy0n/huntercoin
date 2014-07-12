#include "botutil.h"

 Game::Coord BotUtility::getEmptyCoinCoord(const Game::GameState &gameState,const Game::Coord &coord,int within) 
 {
	std::vector<Game::Coord> coords;
    //assign coinspawns by color
    //offset = colorint*4;
    for (int i = 0; i < Game::NUM_HARVEST_AREAS; i++)   
    {
    	for (int a = 0; a < Game::HarvestAreaSizes[i]; a++) 
        {
    		Game::Coord harvest(Game::HarvestAreas[i][2 * a], Game::HarvestAreas[i][2 * a + 1]);
    		if (BotUtility::getDistance(harvest,coord) <= within && BotUtility::isEmpty(harvest,gameState)) 
            {
    			coords.push_back(harvest);
    		}
    	}
    }
    if (coords.size() > 0) 
    {
    	return coords[rand() % coords.size()];
    }
    return Game::Coord(-1,-1);
}