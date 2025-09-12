#include <Geode/Geode.hpp>
#include "Client.hpp"
using namespace geode::prelude;

SettingsManager* SettingsManager::instance = nullptr;

SettingsManager* SettingsManager::get() {
    if (!instance) {
        instance = new SettingsManager();
    }
    return instance;
}

bool SettingsManager::getIfEnabled(const std::string& settingName) {
    auto it = settings.find(settingName);
    if (it != settings.end()) {
        return it->second;
    }
    return Mod::get()->getSavedValue<bool>(settingName, false);
}

void SettingsManager::setEnabled(const std::string& settingName, bool enabled) {
    settings[settingName] = enabled;
    Mod::get()->setSavedValue<bool>(settingName, enabled);
}

void SettingsManager::toggle(const std::string& settingName) {
    bool current = getIfEnabled(settingName);
    setEnabled(settingName, !current);
}

float SettingsManager::getFloatValue(const std::string& settingName) {
    auto it = floatSettings.find(settingName);
    if (it != floatSettings.end()) {
        return it->second;
    }
    return Mod::get()->getSavedValue<float>(settingName + "_float", 1.0f);
}

void SettingsManager::setFloatValue(const std::string& settingName, float value) {
    floatSettings[settingName] = value;
    Mod::get()->setSavedValue<float>(settingName + "_float", value);
}

bool SettingsManager::IfFloatset(const std::string& settingName) {
    return Mod::get()->getSavedValue<bool>(settingName + "_floatset", false);
}

void SettingsManager::saveAll() {
    for (auto& pair : settings) {
        (void)Mod::get()->setSavedValue<bool>(pair.first, pair.second);
    }
    for (auto& pair : floatSettings) {
        (void)Mod::get()->setSavedValue<float>(pair.first + "_float", pair.second);
    }
    (void)Mod::get()->saveData();
}

SolidWave* SolidWave::get() {
    static SolidWave instance;
    return &instance;
}

bool SolidWave::getIfEnabled() {
    return SettingsManager::get()->getIfEnabled("SolidWave");
}

void SolidWave::setEnabled(bool enabled) {
    SettingsManager::get()->setEnabled("SolidWave", enabled);
}

void SolidWave::toggle() {
    SettingsManager::get()->toggle("SolidWave");
}

bool SolidWave::IfFloatset() {
    return SettingsManager::get()->IfFloatset("SolidWave");
}

float SolidWave::getFloatValue() {
    return SettingsManager::get()->getFloatValue("SolidWave");
}

void SolidWave::setFloatValue(float value) {
    SettingsManager::get()->setFloatValue("SolidWave", value);
}

SpeedHack* SpeedHack::get() {
    static SpeedHack instance;
    return &instance;
}

bool SpeedHack::getIfEnabled() {
    return SettingsManager::get()->getIfEnabled("SpeedHack");
}

void SpeedHack::setEnabled(bool enabled) {
    SettingsManager::get()->setEnabled("SpeedHack", enabled);
}

void SpeedHack::toggle() {
    SettingsManager::get()->toggle("SpeedHack");
}

bool SpeedHack::IfFloatset() {
    return SettingsManager::get()->IfFloatset("SpeedHack");
}

float SpeedHack::getFloatValue() {
    return SettingsManager::get()->getFloatValue("SpeedHack");
}

void SpeedHack::setFloatValue(float value) {
    SettingsManager::get()->setFloatValue("SpeedHack", value);
}

Noclip* Noclip::get() {
    static Noclip instance;
    return &instance;
}

bool Noclip::getIfEnabled() {
    return SettingsManager::get()->getIfEnabled("Noclip");
}

void Noclip::setEnabled(bool enabled) {
    SettingsManager::get()->setEnabled("Noclip", enabled);
}

void Noclip::toggle() {
    SettingsManager::get()->toggle("Noclip");
}

bool Noclip::IfFloatset() {
    return SettingsManager::get()->IfFloatset("Noclip");
}

float Noclip::getFloatValue() {
    return SettingsManager::get()->getFloatValue("Noclip");
}

void Noclip::setFloatValue(float value) {
    SettingsManager::get()->setFloatValue("Noclip", value);
}
