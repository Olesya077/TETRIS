#include "Score.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

GameScore::GameScore() {
    loadScores();
}

void GameScore::loadScores() {
    highScores.clear();
    std::ifstream file(scoreFile);
    
    if (file.is_open()) {
        HighScore score;
        while (file >> score.playerName >> score.score >> score.date) {
            highScores.push_back(score);
        }
        file.close();
        
        std::sort(highScores.begin(), highScores.end(),
            [](const HighScore& a, const HighScore& b) {
                return a.score > b.score;
            });
    }
}

void GameScore::saveScores() {
    std::ofstream file(scoreFile);
    
    if (file.is_open()) {
        int count = std::min(10, (int)highScores.size());
        for (int i = 0; i < count; i++) {
            file << highScores[i].playerName << " "
                 << highScores[i].score << " "
                 << highScores[i].date << std::endl;
        }
        file.close();
    }
}

void GameScore::addScore(const std::string& name, int score) {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    std::stringstream ss;
    ss << (now->tm_year + 1900) << "-"
       << (now->tm_mon + 1) << "-"
       << now->tm_mday;
    
    HighScore newScore;
    newScore.playerName = name;
    newScore.score = score;
    newScore.date = ss.str();
    
    highScores.push_back(newScore);
    
    std::sort(highScores.begin(), highScores.end(),
        [](const HighScore& a, const HighScore& b) {
            return a.score > b.score;
        });
    
    saveScores();
}

std::vector<HighScore> GameScore::getTopScores(int count) const {
    int num = std::min(count, (int)highScores.size());
    return std::vector<HighScore>(highScores.begin(), highScores.begin() + num);
}

void GameScore::displayScores() const {
    std::cout << "\n========== ТАБЛИЦА РЕКОРДОВ ==========\n";
    std::cout << std::left << std::setw(5) << "№"
              << std::setw(20) << "Имя"
              << std::setw(10) << "Очки"
              << "Дата" << std::endl;
    std::cout << "----------------------------------------\n";
    
    auto topScores = getTopScores(10);
    for (size_t i = 0; i < topScores.size(); i++) {
        std::cout << std::left << std::setw(5) << (i + 1)
                  << std::setw(20) << topScores[i].playerName
                  << std::setw(10) << topScores[i].score
                  << topScores[i].date << std::endl;
    }
    std::cout << "========================================\n";
}