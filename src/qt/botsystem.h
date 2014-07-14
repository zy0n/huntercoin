#ifndef BOTSYSTEM_H
#define BOTSYSTEM_H

#include "botutil.h"

typedef std::vector<BotPlayer> BotNet;
typedef std::map<int, BotTarget> TargetMap;
typedef std::map<std::string, BotTarget> TargetQueue;
typedef std::map<std::string, std::string> PendingMap;

struct BotController
{
    bool enabled;
    BotNet bots;
    TargetMap openTargets;        //stores array of available targets with no owner
    TargetQueue queuedTargets;    //stores array of targets that have been accepted by bot
    PendingMap pendingBots;
    //should do std::multimap<BotPlayer,BotTarget>
    
    IMPLEMENT_SERIALIZE 
    (
        READWRITE(enabled);
        READWRITE(bots);
        READWRITE(openTargets);
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
    std::string transferTo;
    int color;
    Game::PlayerState &player;
    std::vector<Game::Coord> dests;
    BotType type;
    BotStatus status;
    int64 loot;
    int pending;
    bool alive;
    //bool active;
    bool hasMoves;      //has moves to send to network
    
    QueuedPlayerMoves &moves;
    TargetMap targets;
    std::string movestr;
    //add queued moves array here
    
    
    BotPlayer(Game::PlayerID _name, int _color, BotType _type) : 
    name(_name),
    color(_color),
    type(_type),    
    pending(0),
    alive(false),
    //active(false), 
    hasMoves(false)
    { }
    
    //void UpdatePlayer(const Game::PlayerState &_player)
    void UpdatePlayer(const Game::GameState &state)
    {
        status = (BotStatus)ManageNamesPage::getBotStatus(name);
        Game::PlayerStateMap::const_iterator mi = gameState.players.find(bot.name);
        if(mi == gameState.players.end())
        {
            alive = false;
            active = false;
            sendMove = false;
            return;
        }
        player = state.players.find(name)->second;
        color = player.color;   
        loot = player.loot.nAmount;
        alive = true;
    }
    
    bool SpawnBot()
    {
        pending = 0;
        targets.clear();
        return ManageNamesPage::SpawnBot(name, color);
    }
    
    void Think(TargetMap &targets)
    {
        /*          INFO
         * active = true;          //task has been assigned to bot
         * active = false;         //no task for bot
         * 
         * assigns player with moves in the move array
         * moves[character.index]
        */
       
        if(active)
        {   
        /*          INFO
         * calculates if moves need to be made
         * can call for assistance from nearby bots
         * sends requests/info updates to botnet
         *  * protection
         *  * assigns 'player' hits (dont make this public) use as bot-service for people (people pay for auto-services/rent bots)
         *  * hostile player detection/memory
         *  * timed updates ie: info of where targets will be at what time, and when to attack/spawn/head there
         */
            BOOST_FOREACH(PAIRTYPE(int, Game::CharacterState) &c, player.characters)
            {
                //determine if character already working on a task
                /*
                 * Check bot type
                 * assign task accordingly
                 * 
                 * assign tasks to bots based on quickest completion time
                 * if bot.char already has task, and new task is assigned
                 *          -this happens because bot.char will complete the task the fastest
                 *          -chars current task will be re-assigned to another bot, or be stored for
                 *           reassignment
                 * 
                 * 
                 * 
                 * loop targets & use logic to determine which target to assign
                 * 
                 * 
                 * need to assign bot characters index's so they're indexed properly in the targets[idx]
                 * 
                 */
                

                
                
                QueuedMove move;
                
                /*
                 * FIND BEST MOFLUFFIN MOVES TO TARGET
                 * 
                 */
                
                
                
                moves[c.first] = move;
            }
        }
        
        
    }
    
    std::string GetMoveString()
    {
        json_spirit::Object json;
        BOOST_FOREACH(const PAIRTYPE(int, QueuedMove)& item, moves)
            json.push_back(json_spirit::Pair(strprintf("%d", item.first), item.second.ToJsonValue()));
        return json_spirit::write_string(json_spirit::Value(json), false);
    }
    
};

struct BotTarget
{
    std::string owner;
    Game::Coord coord;
    BotTargetType type;
    int64 value;
    bool remove;
    
    BotTarget() : remove(false), value(0) { }
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