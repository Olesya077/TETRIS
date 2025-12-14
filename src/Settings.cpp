/**
 * @file Settings.cpp
 * @brief Реализация системы настроек (Singleton)
 * 
 * Управляет настройками игры, загрузкой/сохранением в файл,
 * переназначением клавиш управления.
 */

#include "Settings.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <iomanip>

Settings* Settings::instance = nullptr;

Settings::Settings() {
    initReservedKeys();
    loadDefaultControls();
    loadSettings();
}

void Settings::initReservedKeys() {
    reservedKeys = {'1', '2', '3', '4', 's', 'h', 'r', 'n', 'v', 'q'};
}

void Settings::loadDefaultControls() {
    controls["LEFT"] = 'a';
    controls["RIGHT"] = 'd';
    controls["DOWN"] = 's';
    controls["DROP"] = 'w';
    controls["ROTATE"] = ' ';
    controls["PAUSE"] = 'e';
    controls["QUIT"] = 'q';
    
    gameSettings["LEVEL"] = 1;
    gameSettings["SCORE"] = 0;
    gameSettings["LINES_CLEARED"] = 0;
    gameSettings["GRAVITY_SPEED"] = 200000;
}

Settings* Settings::getInstance() {
    if (!instance) {
        instance = new Settings();
    }
    return instance;
}

Settings* Settings::createNewInstance() {
    return new Settings();
}

void Settings::destroyInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

bool Settings::isValidKey(char key) const {
    return std::isprint(key) || key == ' ';
}

bool Settings::isKeyReserved(char key) const {
    return reservedKeys.find(key) != reservedKeys.end();
}

bool Settings::isKeyAvailableForAction(const std::string& action, char key) const {
    if (isKeyReserved(key)) {
        return false;
    }
    
    for (const auto& pair : controls) {
        if (pair.first != action && pair.second == key) {
            return false;
        }
    }
    
    return isValidKey(key);
}

char Settings::getControl(const std::string& action) const {
    auto it = controls.find(action);
    if (it != controls.end()) {
        return it->second;
    }
    return 0;
}

bool Settings::setControl(const std::string& action, char newKey) {
    /**
     * @brief Устанавливает новую клавишу для действия
     * @param action Название действия
     * @param newKey Новая клавиша
     * @return true если клавиша успешно установлена
     * @note Проверяет: 
     *       1. Существование действия
     *       2. Валидность клавиши
     *       3. Занятость клавиши другими действиями
     *       4. Не является ли клавиша зарезервированной
     */
    newKey = std::tolower(newKey);
    
    if (controls.find(action) == controls.end()) {
        return false;
    }
    
    if (!isKeyAvailableForAction(action, newKey)) {
        return false;
    }
    
    controls[action] = newKey;
    saveSettings();
    return true;
}

std::vector<std::string> Settings::getAvailableActions() const {
    std::vector<std::string> actions;
    for (const auto& pair : controls) {
        actions.push_back(pair.first);
    }
    return actions;
}

std::vector<char> Settings::getOccupiedKeys() const {
    std::vector<char> keys;
    for (const auto& pair : controls) {
        keys.push_back(pair.second);
    }
    return keys;
}

int Settings::getLevel() const {
    auto it = gameSettings.find("LEVEL");
    return (it != gameSettings.end()) ? it->second : 1;
}

int Settings::getLinesForNextLevel() const {
    int level = getLevel();
    return level * 10;
}

void Settings::setLevel(int level) {
    gameSettings["LEVEL"] = level;
    gameSettings["GRAVITY_SPEED"] = getSpeedForLevel(level);
    saveSettings();
}

int Settings::getSetting(const std::string& setting) const {
    auto it = gameSettings.find(setting);
    return (it != gameSettings.end()) ? it->second : 0;
}

void Settings::setSetting(const std::string& setting, int value) {
    gameSettings[setting] = value;
    saveSettings();
}

int Settings::getDropPointsForLevel(int level) {
    return 50 * level;
}

int Settings::getSpeedForLevel(int level) {
    int baseSpeed = 200000;
    int minSpeed = 50000;
    int speed = baseSpeed - (level - 1) * 15000;
    return std::max(speed, minSpeed);
}

void Settings::saveSettings() {
    std::ofstream file(settingsFile);
    if (file.is_open()) {
        file << "[CONTROLS]" << std::endl;
        for (const auto& pair : controls) {
            file << pair.first << "=" << pair.second << std::endl;
        }
        
        file << "[SETTINGS]" << std::endl;
        for (const auto& pair : gameSettings) {
            file << pair.first << "=" << pair.second << std::endl;
        }
        
        file.close();
    }
}

void Settings::loadSettings() {
    std::ifstream file(settingsFile);
    if (file.is_open()) {
        std::string line;
        std::string section;
        
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            
            if (line[0] == '[' && line[line.length()-1] == ']') {
                section = line.substr(1, line.length()-2);
                continue;
            }
            
            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                if (section == "CONTROLS" && value.length() == 1) {
                    char c = value[0];
                    bool keyOccupied = false;
                    for (const auto& pair : controls) {
                        if (pair.first != key && pair.second == c) {
                            keyOccupied = true;
                            break;
                        }
                    }
                    
                    if (!keyOccupied) {
                        controls[key] = c;
                    }
                }
                else if (section == "SETTINGS") {
                    try {
                        gameSettings[key] = std::stoi(value);
                    } catch (...) {
                    }
                }
            }
        }
        file.close();
    }
}

void Settings::resetToDefaults() {
    controls.clear();
    gameSettings.clear();
    loadDefaultControls();
    saveSettings();
}