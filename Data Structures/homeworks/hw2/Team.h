#ifndef TEAM_H_INCLUDED
#define TEAM_H_INCLUDED
#include <string>
#include <vector>
#include <iostream>
#include <map>

#include "Player.h"
//#include "Player.cpp"


class Team {
public:
    Team();
    Team(std::string nTeam_name, std::map<std::string, Player> player_arr);
    Team(std::string nTeam_name);

    int getGamesWon() const;
    int getGamesLost() const;
    int getGamesTied() const;
    int getGoals() const;
    int getPenalties() const;
    std::vector<float> getTeamTrendArr() const;
    std::string getTeamName() const;
    std::map<std::string, Player> getPlayerHash() const;
    float getWinPercentage() const;
    float calcSeasonDelta() const;
    std::map<std::string, Player> playerHash;

    void addGameQuality(int numGoals, int numAssists, int numPenalties);
    void addGameStats(std::string result, int nGoals, int nPenalties);
    void addPlayer(Player newPlayer);

private:
    std::string team_name;
    std::vector<float> team_trend_arr;

    int games_won;
    int games_tied;
    int tot_games;
    int goals;
    int penalties;
};

bool isGreaterThan(const Team& t1, const Team& t2);
std::ostream& operator<< (std::ostream& ostr, const Team& t);

#endif // TEAM_H_INCLUDED
