#include "OptionsManager.h"
#include <QDebug>

bool OptionsManager::fileExists(const char* fileName) {
    if (FILE *file = fopen(fileName, "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}
void OptionsManager::Initialize(const char* optionFilePath) {
    if(fileExists(optionFilePath)) {
        std::ifstream optionsFile(optionFilePath);
        std::string key, value;
        bool writingKey, writingValue, writingFinished;
        for(char chr; optionsFile.get(chr); ) {
            if(chr == '@') {
                writingKey = true;
                writingValue = false;
                writingFinished = false;
                key.clear();
                value.clear();
                continue;
            } else if(chr == ':') {
                writingKey = false;
                writingValue = true;
                while(optionsFile.get(chr)) {
                    if(chr != ' ' && chr != '\t') {
                        break;
                    }
                }
            } else if(writingFinished) {
                continue;
            }

            if(chr == '\n' || chr == '\r') {
                if(key.size() != 0 && value.size() != 0) {
                    instance.valuesKeys.insert(std::pair<std::string, std::string>(key, value));
                }
                writingFinished = true;
            }

            if(writingKey) {
                key += chr;
            } else if(writingValue) {
                value += chr;
            }
        }
        if(key.size() != 0 && value.size() != 0) {
            instance.valuesKeys.insert(std::pair<std::string, std::string>(key, value));
        }
    }
}
std::string OptionsManager::OptionValue(const std::string& key) {
    typedef std::map<std::string, std::string>::const_iterator MapIter;
    MapIter iter = instance.valuesKeys.find(key);
    return iter != instance.valuesKeys.cend() ? iter->second : "";
}
std::vector<std::string> OptionsManager::GetAllKeys() {
    std::vector<std::string> keys;
    typedef std::map<std::string, std::string>::const_iterator MapIter;
    for(MapIter iter = instance.valuesKeys.cbegin(); iter != instance.valuesKeys.cend(); ++iter) {
        keys.push_back(iter->first);
    }
    return keys;
}
OptionsManager OptionsManager::instance = OptionsManager();
