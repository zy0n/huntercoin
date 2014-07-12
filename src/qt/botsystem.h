#ifndef BOTSYSTEM_H
#define BOTSYSTEM_H

#include "botutil.h"

typedef std::vector<BotPlayer> BotNet;
typedef std::map<std::string, std::string> PendingMap;

struct BotController
{
    bool enabled;
    BotNet bots;
    std::map<int, BotTarget> botTargets;        //stores array of available/current targets
    PendingMap pendingBots;
    //should do std::multimap<BotPlayer,BotTarget>
    
    IMPLEMENT_SERIALIZE 
    (
        READWRITE(enabled);
        READWRITE(bots);
        READWRITE(botTargets);
        READWRITE(pendingBots);
        
    )
    
    BotController() : enabled(false) { }
    void GameStateUpdated(const Game::GameState &state);
    void UpdateBots(const Game::GameState &state);
    void RemoveTarget(int index);
    void UpdatePending();
    void initBot(std::string name, BotType type)
    {
        BotPlayer newBot = new BotPlayer(name, type);
        bots.push_back(newBot);
    }
};

struct BotPlayer
{
    Game::PlayerID name;
    int color;
    Game::PlayerState &player;
    std::vector<Game::Coord> dests;
    std::multimap<std::string, BotTarget> targets;  
    BotType type;
    BotStatus status;
    int64 loot;
    int pending;
    BotPlayer(Game::PlayerID _name, int _color, BotType _type) : name(_name), color(_color), type(_type), pending(0) { }
    
    //void UpdatePlayer(const Game::PlayerState &_player)
    void UpdatePlayer(const Game::GameState &state)
    {
        //player = _player;
        player = state.players.find(name)->second;
        color = player.color;
        status = (BotStatus)ManageNamesPage::getBotStatus(name);
        loot = player.loot.nAmount;
    }
    
    bool SpawnBot()
    {
        pending = 0;
        targets.clear();
        return ManageNamesPage::SpawnBot(name, color); 
    }
};

struct BotTarget
{
    std::string owner;
    Game::Coord coord;
    BotTargetType type;
    int64 value;
    bool remove;
    
    BotTarget() : remove(false), value(0), coord(Game::Coord(-1,-1)) { }
    
};


enum BotStatus
{
    READY,
    DEAD,
    PENDING
};

enum BotTargetType
{
    COIN,
    HEART,
    PLAYER,
    TILE
};

enum BotType
{
    BOMBER,
    DEFEND,
    GATHER,
    HUNTER
};










#endif //BOTSYSTEM_H