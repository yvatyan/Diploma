#ifndef OPTIONSMANAGER_H
#define OPTIONSMANAGER_H

#include <fstream>
#include <string>
#include <vector>
#include <map>

class OptionsManager {
    public:
        static void Initialize(const char* optionFilePath = "Options.save");
        static std::string OptionValue(const std::string& key);
        static std::vector<std::string> GetAllKeys();
    private:
        static OptionsManager instance;

        std::map<std::string, std::string> valuesKeys;

        static bool fileExists(const char* fileName);
};

#endif // OPTIONSMANAGER_H
