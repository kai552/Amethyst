#include <Geode/Geode.hpp>
#include "GUI.hpp"
using namespace geode::prelude;

bool imguiThemeEnabled = Mod::get()->getSavedValue<bool>("imguiThemeEnabled", false);

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

void SettingsManager::saveAll() {
    for (auto& pair : settings) {
        Mod::get()->setSavedValue<bool>(pair.first, pair.second);
    }
    Mod::get()->saveData();
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
    
    addDefaultUICategoryButton("Player");
    addDefaultUICategoryButton("Creator");
    addDefaultUICategoryButton("Misc");
    addDefaultUICategoryButton("Cosmetic");
    addDefaultUICategoryButton("Settings");
    addDefaultUICategoryButton("Credits");
    
    m_currentCategory = "Player";
    this->setTitle("Player");
    setupCategoryContent("Player");
    
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
        return { "Noclip", "Jump Hack" };
    } else if (categoryStr == "Creator") {
        return { "Copy Hack" };
    } else if (categoryStr == "Misc") {
        return { "Show Hitboxes", "Practice Music Bypass" };
    } else if (categoryStr == "Cosmetic") {
        return { "Rainbow Icon" };
    } else if (categoryStr == "Settings") {
        return { "PlaceHolder" };
    } else if (categoryStr == "Credits") {
        return { "Kai" };
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
