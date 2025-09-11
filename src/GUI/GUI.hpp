#pragma once

#include <Geode/Geode.hpp>
#include <unordered_map>
#include <string>
using namespace geode::prelude;

class SettingsManager {
private:
    static SettingsManager* instance;
    std::unordered_map<std::string, bool> settings;
    
    SettingsManager() = default;

public:
    static SettingsManager* get() {
        if (!instance) {
            instance = new SettingsManager();
        }
        return instance;
    }
    
    bool getIfEnabled(const std::string& settingName) {
        auto it = settings.find(settingName);
        if (it != settings.end()) {
            return it->second;
        }
        return Mod::get()->getSavedValue<bool>(settingName, false);
    }
    
    void setEnabled(const std::string& settingName, bool enabled) {
        settings[settingName] = enabled;
        Mod::get()->setSavedValue<bool>(settingName, enabled);
    }
    
    void toggle(const std::string& settingName) {
        bool current = getIfEnabled(settingName);
        setEnabled(settingName, !current);
    }
    
    void saveAll() {
        for (auto& pair : settings) {
            Mod::get()->setSavedValue<bool>(pair.first, pair.second);
        }
        Mod::get()->saveData();
    }
};
