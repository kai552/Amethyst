#pragma once

#include <Geode/Geode.hpp>
#include <unordered_map>
#include <string>
using namespace geode::prelude;

class SettingsManager {
private:
    static SettingsManager* instance;
    std::unordered_map<std::string, bool> settings;
    std::unordered_map<std::string, float> floatSettings;
    
    SettingsManager() = default;

public:
    static SettingsManager* get();
    bool getIfEnabled(const std::string& settingName);
    void setEnabled(const std::string& settingName, bool enabled);
    void toggle(const std::string& settingName);
    
    float getFloatValue(const std::string& settingName);
    void setFloatValue(const std::string& settingName, float value);
    bool IfFloatset(const std::string& settingName);
    
    void saveAll();
};

class SolidWave {
public:
    static SolidWave* get();
    bool getIfEnabled();
    void setEnabled(bool enabled);
    void toggle();
    bool IfFloatset();
    float getFloatValue();
    void setFloatValue(float value);
};

class SpeedHack {
public:
    static SpeedHack* get();
    bool getIfEnabled();
    void setEnabled(bool enabled);
    void toggle();
    bool IfFloatset();
    float getFloatValue();
    void setFloatValue(float value);
};

class Noclip {
public:
    static Noclip* get();
    bool getIfEnabled();
    void setEnabled(bool enabled);
    void toggle();
    bool IfFloatset();
    float getFloatValue();
    void setFloatValue(float value);
};
