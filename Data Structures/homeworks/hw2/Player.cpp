#include <iostream>
#include <string>
#include "Player.h"

Player:: Player(){
    player_name = "";
    team_name = "";
    games_played = 0;
    goals = 0;
    assists = 0;
    penalties  = 0;
}

Player:: Player(std::string nPlayer_name, std::string nTeam_name){
    player_name =  nPlayer_name;
    team_name = nTeam_name;
    games_played = 0;
    goals = 0;
    assists = 0;
    penalties  = 0;
}

std::string Player:: getPlayerName() const{
    return player_name;
}

std::string Player:: getTeamName() const{
    return team_name;
}

int Player:: getGamesPlayed() const{
    return games_played;
}

int Player:: getGoals() const{
    return goals;
}

void Player::addGoal(){
    goals += 1;
}

int Player:: getAssists() const{
    return assists;
}

void Player:: addAssist(){
    assists +=1;
}

int Player:: getPenalties() const{
    return penalties;
}

void Player:: addPenalty(){
    penalties += 1;
}

void Player:: addGameStats(int nGoals, int nAssists, int nPenalties){
    goals += nGoals;
    assists += nAssists;
    penalties += nPenalties;
}

bool isBetterThan(const Player &p1, const Player &p2){
    //std::cout << p1.getPlayerName() << " " << p2.getPlayerName() << std::endl;
    if((p1.getGoals() + p1.getAssists()) > (p2.getGoals() + p2.getAssists())) return true;
    else if ((p1.getGoals() + p1.getAssists()) == (p2.getGoals() + p2.getAssists())){
        if (p1.getPenalties() < p2.getPenalties()) return true;
        else if (p1.getPenalties() == p2.getPenalties()){
            if(p1.getPlayerName().compare(p2.getPlayerName()) < 0) return true;
        }
    }
    return false;
}
