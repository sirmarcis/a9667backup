#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <map>
#include <algorithm>

#include "Team.h"

std::vector<std::vector<std::string> >
loadGameInput(std::ifstream& in_str){
    std::vector<std::vector<std::string> > game_stats_arr;
    std::vector<std::string> curr_game_stats;
    std::string in_line;
    int cntToEndGame = 6;
    while(!in_str.eof()){
        in_str >> in_line;
        if(in_line == "FINAL"){
            cntToEndGame = 0;
            curr_game_stats.push_back(in_line);
        }
        else if(cntToEndGame == 3){
            curr_game_stats.push_back(in_line);
            game_stats_arr.push_back(curr_game_stats);
            curr_game_stats.clear();
            cntToEndGame += 1;
        }
        else{
            cntToEndGame += 1;
            curr_game_stats.push_back(in_line);
        }
    }
    return game_stats_arr;
}

void
addGoal(Team &team1, Team &team2, int &team1Goals, int &team2Goals, int &currTeam, int h,
        bool &lookForAssists,
        std::vector<std::string> game_stats,
        std::vector<std::string> &playerNameArr){
    lookForAssists = true;
                std::string teamName = game_stats[(h - 1)];
                std::string playerName = game_stats[(h + 1)];
                if (playerName.size() > 23){
                    playerName = playerName.substr(13);
                }
                Player scoringPlayer;
                if(team1.getTeamName() == teamName){
                    team1Goals += 1;
                    std::map<std::string, Player> teamPlayerHash = team1.playerHash;
                    scoringPlayer = teamPlayerHash[playerName];
                    if(scoringPlayer.getPlayerName() == ""){
                        playerNameArr.push_back(playerName);
                        scoringPlayer = Player(playerName, teamName);
                        scoringPlayer.addGoal();
                    }
                    else scoringPlayer.addGoal();
                    teamPlayerHash[playerName] = scoringPlayer;
                    currTeam = 1;
                    team1.playerHash = teamPlayerHash;
                }
                else{
                    team2Goals += 1;
                    std::map<std::string, Player> teamPlayerHash = team2.playerHash;
                    scoringPlayer = teamPlayerHash[playerName];
                    if(scoringPlayer.getPlayerName() == ""){
                        playerNameArr.push_back(playerName);
                        scoringPlayer = Player(playerName, teamName);
                        scoringPlayer.addGoal();
                    }
                    else scoringPlayer.addGoal();
                    teamPlayerHash[playerName] = scoringPlayer;
                    currTeam = 2;
                    team2.playerHash = teamPlayerHash;
                }
}

void
addPenalty(Team &team1, Team &team2, int &team1Penalties, int &team2Penalties, int h, std::vector<std::string> game_stats,
    std::vector<std::string> &playerNameArr){
    if(!(game_stats[(h + 1)] == "BENCH")){
        std::string teamName = game_stats[(h - 1)];
        std::string playerName = game_stats[(h + 1)];
        Player scoringPlayer;
        if(team1.getTeamName() == teamName){
            team1Penalties += 1;
            std::map<std::string, Player> teamPlayerHash = team1.playerHash;
            scoringPlayer = teamPlayerHash[playerName];
            if(scoringPlayer.getPlayerName() == ""){
                playerNameArr.push_back(playerName);
                scoringPlayer = Player(playerName, teamName);
                scoringPlayer.addPenalty();
            }
            else scoringPlayer.addPenalty();
            teamPlayerHash[playerName] = scoringPlayer;
            team1.playerHash = teamPlayerHash;
        }
        else{
            team2Penalties += 1;
            std::map<std::string, Player> teamPlayerHash = team2.playerHash;
            scoringPlayer = teamPlayerHash[playerName];
            if(scoringPlayer.getPlayerName() == ""){
                playerNameArr.push_back(playerName);
                scoringPlayer = Player(playerName, teamName);
                scoringPlayer.addPenalty();
            }
            else scoringPlayer.addPenalty();
            teamPlayerHash[playerName] = scoringPlayer;
            team2.playerHash = teamPlayerHash;
        }
    }
    else{
        std::string teamName = game_stats[(h - 1)];
        if(team1.getTeamName() == teamName){
            team1Penalties += 1;
        }
        else{
            team2Penalties += 1;
        }
    }
}

int
addAssists(std::vector<std::string> assistNameArr, Team &scoringTeam, std::vector<std::string> &playerNameArr){
    int numAssists = 0;
    for (int x = 1; x < assistNameArr.size(); x++){
        numAssists += 1;
        std::string playerName = assistNameArr[x];
        std::map<std::string, Player> teamPlayerHash = scoringTeam.playerHash;
        if(teamPlayerHash[playerName].getPlayerName() == ""){
            teamPlayerHash[playerName] = Player(playerName, scoringTeam.getTeamName());
            teamPlayerHash[playerName].addAssist();
            playerNameArr.push_back(playerName);
        }
        else teamPlayerHash[playerName].addAssist();
        scoringTeam.playerHash = teamPlayerHash;
    }
    return numAssists;
}

void
parsePeriodStats(Team &team1, Team &team2, std::vector<std::string> game_stats, std::vector<std::string> &playerNameArr){
    std::vector<int> periodPosArr;
    for(int x = 0; x < game_stats.size(); x++){
        if(game_stats[x] == "PERIOD" || game_stats[x] == "OVERTIME"){
            periodPosArr.push_back((x + 1));
        }
    }
    periodPosArr.push_back(game_stats.size());
    int team1Goals = 0, team2Goals = 0, team1Penalties = 0, team2Penalties = 0, team1Assists = 0, team2Assists = 0;
    int periodStartPos = 0;
    for(int x = 0; x < periodPosArr.size(); x++){
        bool lookForAssists = false;
        int parenBeginPos = 0;
        int currTeam;
        std::vector<std::string> temp;
        for(int h = periodStartPos; h < periodPosArr[x]; h++){
            if(game_stats[h] == ")" && lookForAssists){
                lookForAssists = false;
                if(currTeam == 1){
                    team1Assists += addAssists(temp ,team1, playerNameArr);
                }
                else{
                    team2Assists += addAssists(temp ,team2, playerNameArr);
                }
                temp.clear();
            }
            if(!(game_stats[h] == "(") && lookForAssists) temp.push_back(game_stats[h]);
            if(game_stats[h] == "goal"){
                addGoal(team1, team2, team1Goals, team2Goals, currTeam, h, lookForAssists, game_stats, playerNameArr);
            }
            else if(game_stats[h] == "penalty") addPenalty(team1, team2, team1Penalties, team2Penalties, h, game_stats, playerNameArr);

        }
        periodStartPos = periodPosArr[x];
    }
    team1.addGameQuality(team1Goals, team1Assists, team1Penalties);
    team2.addGameQuality(team2Goals, team2Assists, team2Penalties);
    if(team1Goals > team2Goals){
        team1.addGameStats("win", team1Goals, team1Penalties);
        team2.addGameStats("loss", team2Goals, team2Penalties);
    }
    else if (team1Goals == team2Goals){
        team1.addGameStats("tie", team1Goals, team1Penalties);
        team2.addGameStats("tie", team2Goals, team2Penalties);
    }
    else{
       team1.addGameStats("loss", team1Goals, team1Penalties);
       team2.addGameStats("win", team2Goals, team2Penalties);
    }
}

std::vector<std::string>
stashTeamStats(std::vector<std::vector<std::string> > game_stats_arr, std::map<std::string, Team>& team_hash, std::vector<std::string> &playerNameArr){
    std::vector<std::string> teamNameArr;
    for(int x = 0; x < game_stats_arr.size(); x++){
        std::vector<std::string> game_stats = game_stats_arr[x];
        std::string teamName1 = game_stats[4], teamName2 = game_stats[6];
        Team team1, team2;
        if(team_hash[teamName1].getTeamName() == ""){
            team1 = Team(teamName1);
            team_hash[teamName1] = team1;
            teamNameArr.push_back(teamName1);
        }
        else team1 = team_hash[teamName1];
        if(team_hash[teamName2].getTeamName() == ""){
            team2 = Team(teamName2);
            team_hash[teamName2] = team2;
            teamNameArr.push_back(teamName2);
        }
        else team2 = team_hash[teamName2];
        parsePeriodStats(team1, team2, game_stats, playerNameArr);
        team_hash[teamName1] = team1; team_hash[teamName2] = team2;
    }
    return teamNameArr;
}

int
getBestOffset(std::vector<Team>& newTeamArr){
    int orig_offset = 15;
    for (int x = 0; x < newTeamArr.size(); x++){
        std::string teamName = newTeamArr[x].getTeamName();
        if(teamName.size() > orig_offset) orig_offset = teamName.size() + 2;
    }
    return orig_offset;
}

void
produceFirstSpreadsheet(std::ofstream& out_str, std::vector<Team>& newTeamArr){
    int offset = getBestOffset(newTeamArr);
    out_str << "Team Name" << std::setw(offset - 9) << "W" << std::setw(4) << "L" << std::setw(4) << "T" << std::setw(7) << "Win%" << std::setw(7)
    << "Goals" << std::setw(11) << "Penalties"<< std::endl;
    for(int x  = 0; x < newTeamArr.size(); x++){
        Team t1 = newTeamArr[x];
        out_str << std::setw(0);
        out_str << t1.getTeamName() << std::setw(offset - t1.getTeamName().size());
        out_str << t1.getGamesWon() << std::setw(4);
        out_str << t1.getGamesLost() << std::setw(4);
        out_str << t1.getGamesTied() << std::setw(7);
        out_str.precision(2);
        out_str << std::fixed << t1.getWinPercentage() << std::setw(7);
        out_str << t1.getGoals() << std::setw(11);
        out_str << t1.getPenalties() << "\n";
    }
}

void
getTeamAndPlayerOffsets(std::vector<Player>& newPlayerArr, int &playerOffset, int &teamOffset){
    for(int x = 0; x < newPlayerArr.size(); x++){
        std::string playerName = newPlayerArr[x].getPlayerName(), teamName = newPlayerArr[x].getTeamName();
        if(playerName.size() > playerOffset) playerOffset = playerName.size() + 2;
        if(teamName.size() > teamOffset) teamOffset = teamName.size() + 2;
    }
}

void
produceSecondSpreadsheet (std::ofstream& out_str, std::vector<Player>& newPlayerArr){
    int playerOffset = 20, teamOffset = 18;
    getTeamAndPlayerOffsets(newPlayerArr ,playerOffset, teamOffset);
    out_str << std::endl;
    out_str << "Player Name" << std::setw(playerOffset - 7)  << "Team" << std::setw(teamOffset - 4) << "Goals" <<
    std::setw(9) << "Assists" << std::setw(11) << "Penalties" << std::endl;
    for(int x = 0; x < newPlayerArr.size(); x++){
        Player p1 = newPlayerArr[x];
        out_str << p1.getPlayerName() << std::setw((playerOffset - p1.getPlayerName().size()) + p1.getTeamName().size());
        out_str << p1.getTeamName()  << std::setw((teamOffset - p1.getTeamName().size()));
        out_str << p1.getGoals() << std::setw(9);
        out_str << p1.getAssists() << std::setw(11);
        out_str << p1.getPenalties() << "\n";
    }
}

void
produceThirdSpreadsheet(std::ofstream& out_str, std::vector<Team>& newTeamArr){
    out_str <<std::endl;
    for(int x = 0; x < newTeamArr.size(); x++){
        Team t1 = newTeamArr[x];
        float teamDelta = t1.calcSeasonDelta() * 100;
        if(teamDelta > 0)
            out_str << t1.getTeamName() << " improved as a team as the season progressed, scoring more goals and getting less penalties by "
            << teamDelta << "%" <<std::endl;
        else
            out_str << t1.getTeamName() << " got worse as a team as the season progressed, earning more penalties while scoring less goals by "
            << teamDelta << "%" << std::endl;
    }
}

int
main(int argc, char* argv[]){
    std::ifstream in_str(argv[1]);
    if(!in_str){
        std::cerr << "Could not open " << argv[1] << "to read\n";
        return 1;
    }
    std::ofstream out_str(argv[2]);
    if (!out_str) {
        std::cerr << "Could not open " << argv[2] << " to write\n";
        return 1;
    }
    std::vector<std::vector<std::string> > game_stats_arr = loadGameInput(in_str);
    std::map<std::string, Team> teamHash;
    std::vector<std::string> playerNameArr, teamNameArr;
    teamNameArr = stashTeamStats(game_stats_arr, teamHash, playerNameArr);
    std::vector<Team> newTeamArr;
    std::vector<Player> newPlayerArr;
    for(int x  = 0; x < teamNameArr.size(); x++){
        Team t1 = teamHash[teamNameArr[x]];
        newTeamArr.push_back(t1);
        for(int y = 0; y < playerNameArr.size(); y++){
            if(!(t1.playerHash[playerNameArr[y]].getPlayerName() == "")){
                newPlayerArr.push_back(t1.playerHash[playerNameArr[y]]);
            }
        }
    }
    //std::cout << "broken here?" <<std::endl;
    std::sort (newTeamArr.begin(), newTeamArr.end(), isGreaterThan);
    std::sort (newPlayerArr.begin(), newPlayerArr.end(), isBetterThan);
    produceFirstSpreadsheet(out_str, newTeamArr);
    produceSecondSpreadsheet(out_str, newPlayerArr);
    produceThirdSpreadsheet(out_str, newTeamArr);
    return 0;
}
