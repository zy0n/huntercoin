#ifndef GAMEMOVECREATOR_H
#define GAMEMOVECREATOR_H

// Advanced client-side path finding that computes shortest path

#include "gamestate.h"

#include <vector>

std::vector<Game::Coord> FindPath(const Game::Coord &start, const Game::Coord &goal);

struct QueuedMove
{
    Game::WaypointVector waypoints;
    bool destruct;

    QueuedMove() : destruct(false) { }
    
    json_spirit::Value ToJsonValue()
    {
        using namespace json_spirit;
        Object obj;
        if(destruct)
            obj.push_back(Pair("destruct", Value(true)));
        else
        {
            if(waypoints->empty())
                continue;
            
            Array arr;
            if (waypoints->size() == 1)
            {
                arr.push_back((*waypoints)[0].x);
                arr.push_back((*waypoints)[0].y);
            }
            else
            {
                for (size_t i = 1, n = waypoints->size(); i < n; i++)
                {
                    arr.push_back((*waypoints)[i].x);
                    arr.push_back((*waypoints)[i].y);
                }
            }
            obj.push_back(Pair("wp", arr));
        }
        return obj;
    }
};

typedef std::map<int, QueuedMove> QueuedPlayerMoves;
typedef std::map<Game::PlayerID, QueuedPlayerMoves> QueuedMoves;
std::vector<Game::Coord> *UpdateQueuedPath(const Game::CharacterState &ch, QueuedMoves &queuedMoves, const Game::CharacterID &chid);
std::vector<Game::Coord> PathToCharacterWaypoints(const std::vector<Game::Coord> &path);

#endif // GAMEMOVECREATOR_H
