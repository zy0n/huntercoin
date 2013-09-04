#ifndef GAMETX_H
#define GAMETX_H

#include <vector>
#include <string>

// This module manages transactions for in-game events (e.g. player rewards):
// - creates transactions for StepResult
// - parses these transactions and creates string representation (to show in the transaction list)

namespace Game
{
    struct GameState;
    struct StepResult;
}

class CTransaction;
class CNameDB;
class CScript;

// Create resulting game transactions for the given StepResult
bool CreateGameTransactions(CNameDB *pnameDb, const Game::GameState &gameState, const Game::StepResult &stepResult, std::vector<CTransaction> &outvgametx);

// Format human-readable description of a single input of game tx.
// Note: the caller must know the structure of game transactions, to correctly match txin and txout
// By providing nameStart/EndTag you can make player names bold in HTML.
std::string GetGameTxDescription(const CScript &scriptSig, bool fBrief, const char *nameStartTag = "", const char *nameEndTag = "", bool fUseColon = true);

#endif // GAMETX_H