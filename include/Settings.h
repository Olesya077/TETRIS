#ifndef SETTINGS_H
#define SETTINGS_H
#include <string>
#include <map>
#include <vector>
#include <set>

class Settings {
private:
    static Settings* instance;
    std::map<std::string, char> controls;
    std::map<std::string, int> gameSettings;
    std::set<char> reservedKeys;
    std::string settingsFile = "tetris_settings.txt";
    Settings();
    void loadDefaultControls();
    void initReservedKeys();
    bool isValidKey(char key) const;
    
public:
    static Settings* getInstance();
    static Settings* createNewInstance();
    static void destroyInstance();
    char getControl(const std::string& action) const;
    bool setControl(const std::string& action, char newKey);
    std::vector<std::string> getAvailableActions() const;
    std::vector<char> getOccupiedKeys() const;
    std::set<char> getReservedKeys() const { return reservedKeys; }
    int getLevel() const;
    int getLinesForNextLevel() const;
    void setLevel(int level);
    int getSetting(const std::string& setting) const;
    void setSetting(const std::string& setting, int value);
    
    void saveSettings();
    void loadSettings();
    void resetToDefaults();
    static int getDropPointsForLevel(int level);
    static int getSpeedForLevel(int level);
    bool isKeyReserved(char key) const;
    bool isKeyAvailableForAction(const std::string& action, char key) const;
};

#endif