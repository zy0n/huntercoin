#include "botutil.h"



/*  BOT SYSTEM TODO/BRAINSTORM:
    
    Bot controller
        -relay's update info to botnet
        -handles group logic
        -spawns bots
    
    BotNet <actual array of bots>
        -targets
            *players
            *coins
            *hearts
        -updates/info
            *info about other players
            *squad fighting tactics
            *squad gathering
        
        
    Bots
        individual logic based on the needs of the botnet
        
    
*/

//BotController core = new BotController();

//BotSystem() : { }


void BotController::RemoveTarget(int index)
{
    openTargets.erase(index);
}

void BotController::UpdateBots(const Game::GameState &state)
{
    BOOST_FOREACH(BotPlayer &bot, bots)
    {
        bot.UpdatePlayer(state);
        //when updating bot check to see if bot is still 'owned' by this client
        //bot name can be 'stolen' in game when death happens
        //status will be BotStatus::DEAD but player will be found in gamestate
    }
}

void BotController::UpdatePending()
{
    PendingMap pendingTemp;
    json_spirit::Array junk;
    json_spirit::Value pendings = name_pending(junk, false);
    json_spirit::Array pendingsArr = pendings.get_array();
    for (int p = 0; p < pendingsArr.size(); p++) {
    	json_spirit::Object pendingObj = pendingsArr[p].get_obj();
    	json_spirit::Pair isMine = pendingObj[4];
    	if (isMine.value_.get_bool()) 
        {
    		json_spirit::Pair nameObj = pendingObj[0];
    		json_spirit::Pair txObj = pendingObj[1];
    		pendingTemp[nameObj.value_.get_str()] = txObj.value_.get_str();
       	}
    }
    pendingBots = pendingTemp;
}

void BotController::GameStateUpdated(const Game::GameState &state)
{
    UpdateBots(state);
    UpdatePending();
    
    /*
        Update Targets
            clean up target array via destroy on nextblock system
            so players that have targets that are no longer targets can get new targets
    */

    BOOST_FOREACH(PAIRTYPE(std::string, BotTarget) &targ,openTargets) 
    {
        if(!targ->second.remove)
            continue;
        openTargets.erase(targ->first);
    }

    BOOST_FOREACH(BotPlayer &bot, bots)
    {
        //bot.UpdatePlayer(state);
        if(bot.status == BotStatus.DEAD)
        {
            BOOST_FOREACH(PAIRTYPE(std::string, BotTarget) &targ, openTargets)   //removes targets for dead players
            {
                if(bot.name != targ->second.owner)
                    continue;
                openTargets.erase(targ->first);
            }
            
        }
        //Removes stale pending TX
        if(pendingBots.find(bot.name) != pendingBots.end())
        {
            bot.pending++;
            if(bot.pending > 20)
            {
                bot.pending = 0;
                ManageNamesPage::removePendingTx(QString::fromStdString(pendingBots[bot.name]));
            }
            continue;
        }
        
        if(!bot.alive)
        {
            //player in nametab but not in gamestate
            if(bot.status == BotStatus.PENDING)
            {
                //NAME_PENDING
            }
            else
            {
                //check balance against minimum
                bot.SpawnBot();
                
            }
            continue;  
        }
        
        bot.Think(openTargets);     
        //assigns bot a task based on needs of botnet
        //calculates moves if needed,  player/coin/heart targets, offensive/defensive moves

        if(!bot.active)     //has no new moves to send || bot isn't owned/needed by this BotNet
            continue;
            
            
        //handle move limit reached
        ManageNamesPage::SendMove(bot);

        /*       
        bool sendMove = false;
        BOOST_FOREACH(const PAIRTYPE(int, Game::CharacterState) &charState, bot.player.characters)
        {
            if(botMoves.find(charState.first) != botMoves.end())
            {
                //new moves calculated
                if(botMoves[charState.first].destruct)
                {
                    queuedMoves[bot.name][charState.first].destruct = true;
                    sendMove = true;
                    continue;
                }
                if (botMoves[charState.first].coords.size() != 0)
                {
                    //bot has waypoints debug display
                }
                //find valid path to target
                
                Game::WaypointVector &cwp = queuedMoves[bot.name][charState.first].waypoints;
                cwp.clear();
                
                Game::Coord startCoord = charState.second.coord;
                bool validCoord = true;
                for (int i = 0; i < botMoves[charState.first].coords.size(); i++)
                {
                    Game::Coord targetCoord = botMoves[charState.first].coords[i];
                    Game::WaypointVector wp = FindPath(startCoord, targetCoord);
                    if(wp.empty())
                    {
                        //invalid coords
                        break;
                    }
                    cwp = wp;
                }
                
            }
            else
            {
                //No moves for character
            }
        }
        
        if(sendMove)
        {
            //send moves for player to network
            //increment movecounter
        }
        */
        
    }


    
    for (BotNet::iterator bot = bots.begin() ; bot != bots.end(); ++bot)
    {
        //unsure where to update player
        //bot.UpdatePlayer(state.players.find(bot.name)->second);
        //use this switch in bot.Think() logic for determining which moves to send
        switch(bot.type)
        {
            case BotType.BOMBER:
                /*
                    teams of 2 hunters
                    goes to target
                    explodes one hunter, other returns to base
                    recycles into queue of solo hunters
                */
                break;
            case BotType.DEFEND:
                /*
                    Teams of bots will radially defend <location> or <bot>
                    bots determine incoming targets
                    predict incoming target eta
                    intercept & destruct
                */
                break;
            case BotType.GATHER:
                /*
                    Teams of bots will gather coins within a radius of <location>
                    locate coins that nobody else can reach faster than you
                    
                */
                break;
            case BotType.HUNTER:
                /*
                    These bots do nothing but kill
                    Teams of 2 or more hunters
                    search out valuable targets
                    intercept & kill
                */
                break;
                
            default: break;
        }            
    }
    
    
    
    
    
    
    
    
}



