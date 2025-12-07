#ifndef SCORESYSTEM_H
#define SCORESYSTEM_H

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

struct HighScore {
    std::string playerName;
    int score;
    std::string date;
};

class GameScore {
private:
    std::vector<HighScore> highScores;
    std::string scoreFile = "tetris_scores.txt";
    
public:
    GameScore();
    void loadScores();
    void saveScores();
    void addScore(const std::string& name, int score);
    void displayScores() const;
    std::vector<HighScore> getTopScores(int count = 10) const;
};

#endif