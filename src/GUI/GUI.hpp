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

class SolidWave {
public:
    static SolidWave* get() {
        static SolidWave instance;
        return &instance;
    }
    
    bool getIfEnabled() {
        return SettingsManager::get()->getIfEnabled("SolidWave");
    }
    
    void setEnabled(bool enabled) {
        SettingsManager::get()->setEnabled("SolidWave", enabled);
    }
    
    void toggle() {
        SettingsManager::get()->toggle("SolidWave");
    }
};

class SpeedHack {
public:
    static SpeedHack* get() {
        static SpeedHack instance;
        return &instance;
    }
    
    bool getIfEnabled() {
        return SettingsManager::get()->getIfEnabled("SpeedHack");
    }
    
    void setEnabled(bool enabled) {
        SettingsManager::get()->setEnabled("SpeedHack", enabled);
    }
    
    void toggle() {
        SettingsManager::get()->toggle("SpeedHack");
    }
};

class Noclip {
public:
    static Noclip* get() {
        static Noclip instance;
        return &instance;
    }
    
    bool getIfEnabled() {
        return SettingsManager::get()->getIfEnabled("Noclip");
    }
    
    void setEnabled(bool enabled) {
        SettingsManager::get()->setEnabled("Noclip", enabled);
    }
    
    void toggle() {
        SettingsManager::get()->toggle("Noclip");
    }
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

#include <Geode/Geode.hpp>
#include "GUI.hpp"
using namespace geode::prelude;

bool imguiThemeEnabled = Mod::get()->getSavedValue<bool>("imguiThemeEnabled", false);

bool UI::setup() {
    auto popupSize = this->getContentSize();
    
    m_bottomButtonsMenu = CCMenu::create();
    m_bottomButtonsMenu->setLayout(RowLayout::create());
    m_bottomButtonsMenu->setContentSize({popupSize.width, 25.f});
    m_mainLayer->addChildAtPosition(m_bottomButtonsMenu, Anchor::Bottom, {0.f, 20.f});
    
    m_scrollView = CCScrollView::create({popupSize.width - 20.f, popupSize.height - 80.f});
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_mainLayer->addChildAtPosition(m_scrollView, Anchor::Center, {0.f, 10.f});
    
    m_contentMenu = CCMenu::create();
    m_contentMenu->setLayout(ColumnLayout::create()
        ->setAxisAlignment(AxisAlignment::Start)
        ->setCrossAxisAlignment(AxisAlignment::Start)
        ->setGap(10.f));
    
    if (GUINoModMenu) {
        
    } else {
        addDefaultUICategoryButton("Player");
        addDefaultUICategoryButton("Creator");
        addDefaultUICategoryButton("Misc");
        addDefaultUICategoryButton("Cosmetic");
        addDefaultUICategoryButton("Settings");
        addDefaultUICategoryButton("Credits");
        
        m_currentCategory = "Player";
        this->setTitle("Player");
        setupCategoryContent("Player");
    }
    
    return true;
}

void UI::onExit() {
    saveAllStates();
    Popup::onExit();
}

void UI::addDefaultUICategoryButton(const std::string& category) {
    auto button = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_longBtn01_001.png"),
        this,
        menu_selector(UI::onCategoryButtonPressed)
    );
    button->setScale(0.6f);
    auto label = CCLabelBMFont::create(category.c_str(), "bigFont.fnt");
    button->addChild(label);
    label->setPosition(button->getContentSize() / 2);
    label->limitLabelWidth(70.0f, 0.7f, 0.1f);
    button->setUserObject(CCString::create(category));
    m_bottomButtonsMenu->addChild(button);
    m_bottomButtonsMenu->updateLayout();
}

void UI::onCategoryButtonPressed(CCObject* sender) {
    auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto categoryStr = static_cast<CCString*>(button->getUserObject());
    std::string category = categoryStr->getCString();
    
    m_currentCategory = category;
    this->setTitle(category);
    setupCategoryContent(category);
}

std::vector<std::string> UI::getCategoryFields(const std::string& categoryStr) {
    if (categoryStr == "Player") {
        return { "Speed Hack", "Noclip", "Jump Hack", "Auto Jump" };
    } else if (categoryStr == "Creator") {
        return { "No Clip", "Free Zoom", "Grid Snap", "Object Limit Bypass" };
    } else if (categoryStr == "Misc") {
        return { "Show Hitboxes", "Practice Music", "No Death Effect", "Instant Complete", "Unlock All", "Auto Song Download" };
    } else if (categoryStr == "Cosmetic") {
        return { "Rainbow Icon", "Custom Wave Trail", "Hide Player", "Custom Colors" };
    } else if (categoryStr == "Settings") {
        return { "Enable Notifications", "Auto Save", "Debug Mode" };
    } else if (categoryStr == "Credits") {
        return { "Nothing Here." };
    }
    return { "Nothing Here." };
}

void UI::setupCategoryContent(const std::string& category) {
    clearContent();
    
    std::vector<std::string> fields = getCategoryFields(category);
    
    for (size_t i = 0; i < fields.size(); i++) {
        std::string labelText = fields[i];
        std::string checkboxKey = category + "Checkbox" + std::to_string(i);
        
        auto container = CCMenu::create();
        container->setLayout(RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGap(10.f));
        container->setContentSize({350.f, 30.f});
        
        auto checkbox = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(UI::onCheckboxToggled),
            0.8f
        );
        checkbox->toggle(loadCheckboxState(checkboxKey));
        checkbox->setUserObject(CCString::create(checkboxKey));
        
        auto label = CCLabelBMFont::create(labelText.c_str(), "bigFont.fnt");
        label->setScale(0.6f);
        
        container->addChild(checkbox);
        container->addChild(label);
        container->updateLayout();
        
        m_contentMenu->addChild(container);
    }
    
    m_contentMenu->setContentSize({350.f, (float)(fields.size() * 40)});
    m_contentMenu->updateLayout();
    
    m_scrollView->setContentSize(m_contentMenu->getContentSize());
    m_scrollView->addChild(m_contentMenu);
}

void UI::clearContent() {
    if (m_contentMenu) {
        m_scrollView->removeChild(m_contentMenu);
        m_contentMenu = CCMenu::create();
        m_contentMenu->setLayout(ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(AxisAlignment::Start)
            ->setGap(10.f));
    }
}

void UI::onCheckboxToggled(CCObject* sender) {
    auto checkbox = static_cast<CCMenuItemToggler*>(sender);
    auto keyStr = static_cast<CCString*>(checkbox->getUserObject());
    std::string key = keyStr->getCString();
    
    saveCheckboxState(key, checkbox->isToggled());
}

void UI::saveCheckboxState(const std::string& key, bool value) {
    Mod::get()->setSavedValue<bool>(key, value);
}

bool UI::loadCheckboxState(const std::string& key, bool defaultValue) {
    return Mod::get()->getSavedValue<bool>(key, defaultValue);
}

void UI::saveAllStates() {
    Mod::get()->saveData();
}
