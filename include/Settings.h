/**
 * @file Settings.h
 * @brief Заголовочный файл, содержащий объявление класса Settings для управления настройками игры
 */
#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <map>
#include <vector>
#include <set>

/**
 * @brief Класс для управления настройками игры
 * 
 * Реализует паттерн Singleton для глобального доступа к настройкам.
 * Управляет настройками управления, уровнем сложности и другими параметрами.
 */
class Settings {
private:
    static Settings* instance;
    std::map<std::string, char> controls;
    std::map<std::string, int> gameSettings;
    std::set<char> reservedKeys;
    std::string settingsFile = "tetris_settings.txt";
    
    /**
     * @brief Приватный конструктор
     */
    Settings();
    
    /**
     * @brief Загружает настройки управления по умолчанию
     */
    void loadDefaultControls();
    
    /**
     * @brief Инициализирует набор зарезервированных клавиш
     */
    void initReservedKeys();
    
    /**
     * @brief Проверяет валидность клавиши
     * @return true если клавиша может быть использована для управления
     */
    bool isValidKey(char key) const;
    
public:
    /**
     * @brief Возвращает единственный экземпляр настроек
     * @return Указатель на экземпляр Settings
     * @note Создает новый экземпляр при первом вызове
     */
    static Settings* getInstance();
    
    /**
     * @brief Создает новый экземпляр настроек
     * @return Указатель на новый экземпляр
     */
    static Settings* createNewInstance();
    
    /**
     * @brief Уничтожает единственный экземпляр
     * @note Освобождает память, выделенную под экземпляр
     */
    static void destroyInstance();
    
    /**
     * @brief Возвращает клавишу для указанного действия
     * @param action Название действия (например, "LEFT", "ROTATE")
     * @return Клавиша, назначенная действию
     */
    char getControl(const std::string& action) const;
    
    /**
     * @brief Устанавливает новую клавишу для действия
     * @param action Название действия
     * @param newKey Новая клавиша
     * @return true если клавиша успешно установлена
     */
    bool setControl(const std::string& action, char newKey);
    
    /**
     * @brief Возвращает список доступных действий
     * @return Вектор названий действий
     */
    std::vector<std::string> getAvailableActions() const;
    
    /**
     * @brief Возвращает список занятых клавиш
     * @return Вектор клавиш, уже назначенных действиям
     */
    std::vector<char> getOccupiedKeys() const;
    
    /**
     * @brief Возвращает набор зарезервированных клавиш
     * @return Множество зарезервированных клавиш
     */
    std::set<char> getReservedKeys() const { return reservedKeys; }
    
    /**
     * @brief Возвращает текущий уровень сложности
     * @return Номер уровня (начинается с 1)
     */
    int getLevel() const;
    
    /**
     * @brief Возвращает количество линий до следующего уровня
     * @return Количество линий, которое нужно очистить для повышения уровня
     */
    int getLinesForNextLevel() const;
    
    /**
     * @brief Устанавливает уровень сложности
     * @param level Новый уровень
     */
    void setLevel(int level);
    
    /**
     * @brief Возвращает значение настройки
     * @param setting Название настройки
     * @return Значение настройки или 0 если не найдено
     */
    int getSetting(const std::string& setting) const;
    
    /**
     * @brief Устанавливает значение настройки
     * @param setting Название настройки
     * @param value Новое значение
     */
    void setSetting(const std::string& setting, int value);
    
    /**
     * @brief Сохраняет настройки в файл
     */
    void saveSettings();
    
    /**
     * @brief Загружает настройки из файла
     * @note Использует значения по умолчанию, если файл не существует
     */
    void loadSettings();
    
    /**
     * @brief Сбрасывает все настройки к значениям по умолчанию
     */
    void resetToDefaults();
    
    /**
     * @brief Возвращает количество очков за дроп на указанном уровне
     * @return Очки за клетку при быстром падении
     */
    static int getDropPointsForLevel(int level);
    
    /**
     * @brief Возвращает скорость падения для указанного уровня
     * @return Скорость падения в микросекундах
     */
    static int getSpeedForLevel(int level);
    
    /**
     * @brief Проверяет, зарезервирована ли клавиша
     * @return true если клавиша зарезервирована
     */
    bool isKeyReserved(char key) const;
    
    /**
     * @brief Проверяет, доступна ли клавиша для указанного действия
     * @return true если клавиша доступна для назначения
     */
    bool isKeyAvailableForAction(const std::string& action, char key) const;
};

#endif