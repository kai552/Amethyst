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
    static SettingsManager* get();
    bool getIfEnabled(const std::string& settingName);
    void setEnabled(const std::string& settingName, bool enabled);
    void toggle(const std::string& settingName);
    void saveAll();
};

class SolidWave {
public:
    static SolidWave* get();
    bool getIfEnabled();
    void setEnabled(bool enabled);
    void toggle();
};

class SpeedHack {
public:
    static SpeedHack* get();
    bool getIfEnabled();
    void setEnabled(bool enabled);
    void toggle();
};

class Noclip {
public:
    static Noclip* get();
    bool getIfEnabled();
    void setEnabled(bool enabled);
    void toggle();
};

enum class Category {
    Player,
    Creator,
    Misc,
    Cosmetic,
    Settings,
    Credits
};

class UI : public geode::Popup<> {
protected:
    CCMenu* m_bottomButtonsMenu;
    CCScrollView* m_scrollView;
    CCMenu* m_contentMenu;
    std::string m_currentCategory;

    bool setup() override;
    void onExit() override;

    void addDefaultUICategoryButton(const std::string& category);
    void addLabel(const std::string& category);
    void onCategoryButtonPressed(CCObject* sender);
    void onCheckboxToggled(CCObject* sender);
    void setupCategoryContent(const std::string& category);
    void clearContent();
    
    void saveCheckboxState(const std::string& key, bool value);
    bool loadCheckboxState(const std::string& key, bool defaultValue = false);
    void saveAllStates();
    std::vector<std::string> getCategoryFields(const std::string& categoryStr);
public:
    static UI* create() {
        auto ret = new UI();
        if (ret && ret->initAnchored(440.f, 250.f)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};
