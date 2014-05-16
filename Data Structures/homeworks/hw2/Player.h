#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include <string>

class Player{
public:
    Player();
    Player(std::string nPlayer_name, std::string nTeam_name);

    std::string getPlayerName() const;
    std::string getTeamName() const;
    int getGamesPlayed() const;
    int getGoals() const;
    int getAssists() const;
    int getPenalties() const;

    void addGoal();
    void addAssist();
    void addPenalty();
    void addGameStats(int nGoals, int nAssists, int nPenalties);
private:
    std::string player_name;
    std::string team_name;
    int games_played;
    int goals;
    int assists;
    int penalties;
};

bool isBetterThan(const Player &p1, const Player &p2);

#endif // PLAYER_H_INCLUDED
