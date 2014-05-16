#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "math.h"

#include "Team.h"
//#include "Player.h"


Team:: Team(){
    team_name = std::string("");
    games_won = 0;
    games_tied = 0;
    tot_games = 0;
    goals = 0;
    penalties = 0;
}

Team:: Team(std::string nTeam_name, std::map<std::string, Player> player_hash){
    team_name = nTeam_name;
    playerHash = player_hash;
    games_won = 0;
    games_tied = 0;
    tot_games = 0;
    goals = 0;
    penalties = 0;
}
Team::Team(std::string nTeam_name){
    team_name = nTeam_name;
    games_won = 0;
    games_tied = 0;
    tot_games = 0;
    goals = 0;
    penalties = 0;
}

int Team:: getGamesWon() const{
    return games_won;
}

int Team:: getGamesLost() const{
    int gamesLost = tot_games - games_won - games_tied;
    return gamesLost;
}

int Team:: getGamesTied() const{
    return games_tied;
}

int Team::getGoals() const{
    return goals;
}

int Team:: getPenalties() const{
    return penalties;
}

std::string Team:: getTeamName() const{
    return team_name;
}

std::map<std::string, Player> Team:: getPlayerHash() const{
    return playerHash;
}

float Team:: getWinPercentage() const{
    float winPercent = (games_won + (.5 * games_tied)) / tot_games;
    winPercent = (winPercent * 100) + .5;
    winPercent = floor(winPercent);
    winPercent = winPercent / 100;
    return winPercent;
}

std::vector<float> Team::getTeamTrendArr() const{
    return team_trend_arr;
}

float Team::calcSeasonDelta() const {
    std::vector<float> deltaArr;
    float prevTrendVal = 0;
    float currDelta = 0;
    for(int x = 0; x < team_trend_arr.size(); x++){
        float currTrendVal = team_trend_arr[x];
        if(!(prevTrendVal == 0)){
            currDelta = (prevTrendVal - currTrendVal) / 2;
            deltaArr.push_back(currDelta);
        }
        prevTrendVal = currTrendVal;
    }
    float runningAvg = 0;
    for(int y = 0; y < deltaArr.size(); y++){
        runningAvg += deltaArr[y];
    }
    runningAvg = runningAvg / deltaArr.size();
    return runningAvg;
}

void Team::addGameQuality(int numGoals, int numAssists, int numPenalties){
    float gameQuality = (numGoals + (.5 * numAssists)) / numPenalties;
    team_trend_arr.push_back(gameQuality);
}

void Team:: addGameStats(std::string result, int nGoals, int nPenalties){
    goals += nGoals;
    penalties += nPenalties;
    if(result == "win"){
        games_won += 1;
    }
    else if(result == "tie"){
        games_tied += 1;
    }
    tot_games += 1;
}

void Team:: addPlayer(Player newPlayer){
    std::string playerName = newPlayer.getPlayerName();
    Player playerBool = playerHash[playerName];
    if(playerBool.getPlayerName() == ""){
        playerHash[newPlayer.getPlayerName()];
    }
}

bool
isGreaterThan(const Team& t1, const Team& t2){
    if(t1.getWinPercentage() > t2.getWinPercentage()){
        return true;
    }
    else if (t1.getWinPercentage() == t2.getWinPercentage()){
        if(t1.getTeamName().compare(t2.getTeamName()) < 0) return true;
        else if (t1.getTeamName().compare(t2.getTeamName()) == 0) std::cout << "ERROR: one team appears twice" <<std::endl;
    }
    else return false;
}

std::ostream& operator<< (std::ostream& ostr, const Team& t) {
    //ostr << t.first() << " " << t.last(); <-- example only, must be modified
    return ostr;
}
