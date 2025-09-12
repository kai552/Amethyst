#include <Geode/Geode.hpp>
#include "GUI.hpp"
using namespace geode::prelude;

bool imguiThemeEnabled = Mod::get()->getSavedValue<bool>("imguiThemeEnabled", false);

bool FloatConfigPopup::setup(std::string settingKey) {
    m_settingKey = settingKey;
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    this->setTitle("Configure " + settingKey);
    
    m_input = CCTextInputNode::create(100.f, 30.f, "1.0", "Thonburi");
    m_input->setLabelPlaceholderColor(ccc3(120, 120, 120));
    m_input->setMaxLabelScale(0.8f);
    
    float currentValue = SettingsManager::get()->getFloatValue(settingKey);
    m_input->setString(std::to_string(currentValue).c_str());
    
    this->m_mainLayer->addChildAtPosition(m_input, Anchor::Center, {0, 20});
    
    auto saveBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_button_01.png"),
        this,
        menu_selector(FloatConfigPopup::onSave)
    );
    
    auto cancelBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_button_06.png"),
        this,
        menu_selector(FloatConfigPopup::onCancel)
    );
    
    auto menu = CCMenu::create();
    menu->addChild(saveBtn);
    menu->addChild(cancelBtn);
    menu->setLayout(RowLayout::create()->setGap(20.f));
    this->m_mainLayer->addChildAtPosition(menu, Anchor::Center, {0, -30});
    
    return true;
}

FloatConfigPopup* FloatConfigPopup::create(const std::string& settingKey) {
    auto ret = new FloatConfigPopup();
    if (ret && ret->initAnchored(200.f, 150.f, settingKey)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void FloatConfigPopup::onSave(CCObject* sender) {
    std::string valueStr = m_input->getString();
    float value = std::stof(valueStr);
    SettingsManager::get()->setFloatValue(m_settingKey, value);
    this->onClose(sender);
}

void FloatConfigPopup::onCancel(CCObject* sender) {
    this->onClose(sender);
}

bool UI::setup() {
    auto popupSize = this->getContentSize();
    
    m_bottomButtonsMenu = CCMenu::create();
    m_bottomButtonsMenu->setLayout(RowLayout::create());
    m_bottomButtonsMenu->setContentSize({popupSize.width, 25.f});
    m_mainLayer->addChildAtPosition(m_bottomButtonsMenu, Anchor::Bottom, {0.f, 20.f});
    
    m_scrollView = CCNode::create();
    m_scrollView->setContentSize({popupSize.width - 20.f, popupSize.height - 80.f});
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

std::vector<UIElement> UI::getCategoryElements(const std::string& categoryStr) {
    if (categoryStr == "Player") {
        return { 
            {"Noclip", UIElementType::Checkbox, "Noclip"},
            {"Jump Hack", UIElementType::Checkbox, "JumpHack"},
            {"Ignore Inputs", UIElementType::Checkbox, "IgnoreInputs"}
        };
    } else if (categoryStr == "Creator") {
        return { 
            {"Custom Objects Bypass", UIElementType::Checkbox, "CustomObjectsBypass"},
            {"Copy Hack", UIElementType::Checkbox, "CopyHack"},
            {"Level Edit", UIElementType::Checkbox, "LevelEdit"}
        };
    } else if (categoryStr == "Misc") {
        return { 
            {"Show Hitboxes", UIElementType::Checkbox, "ShowHitboxes"},
            {"Speedhack", UIElementType::Float, "SpeedHack"},
            {"No Death Effect", UIElementType::Checkbox, "NoDeathEffect"},
            {"Instant Complete", UIElementType::Checkbox, "InstantComplete"},
            {"Unlock All Icons", UIElementType::Checkbox, "UnlockAllIcons"}
        };
    } else if (categoryStr == "Cosmetic") {
        return { 
            {"Rainbow Icon", UIElementType::Checkbox, "RainbowIcon"},
            {"Solid Wave Trail", UIElementType::Float, "SolidWave"}
        };
    } else if (categoryStr == "Credits") {
        return { 
            {"Created by Kai552", UIElementType::Button, "Credits1"},
            {"Thanks to Speedyfriend67", UIElementType::Button, "Credits2"},
            {"Version 1.0.0", UIElementType::Button, "Credits3"}
        };
    }
    return {};
}

std::vector<std::string> UI::getCategoryFields(const std::string& categoryStr) {
    if (categoryStr == "Player") {
        return { "Noclip", "Jump Hack", "Ignore Inputs" };
    } else if (categoryStr == "Creator") {
        return { "Custom Objects Bypass", "Copy Hack", "Level Edit" };
    } else if (categoryStr == "Misc") {
        return { "Show Hitboxes", "Speedhack", "No Death Effect", "Instant Complete", "Unlock All Icons" };
    } else if (categoryStr == "Cosmetic") {
        return { "Rainbow Icon", "Solid Wave Trail" };
    } else if (categoryStr == "Credits") {
        return { "Nothing Here." };
    }
    return { "Nothing Here." };
}

void UI::setupCategoryContent(const std::string& category) {
    clearContent();
    
    std::vector<UIElement> elements = getCategoryElements(category);
    
    for (size_t i = 0; i < elements.size(); i++) {
        auto element = elements[i];
        
        auto container = CCMenu::create();
        container->setLayout(RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGap(10.f));
        container->setContentSize({350.f, 30.f});
        
        if (element.type == UIElementType::Checkbox) {
            auto checkbox = CCMenuItemToggler::createWithStandardSprites(
                this,
                menu_selector(UI::onCheckboxToggled),
                0.8f
            );
            checkbox->toggle(loadCheckboxState(element.key));
            checkbox->setUserObject(CCString::create(element.key));
            container->addChild(checkbox);
            
        } else if (element.type == UIElementType::Button) {
            auto button = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("GJ_button_01.png"),
                this,
                menu_selector(UI::onButtonPressed)
            );
            button->setScale(0.6f);
            button->setUserObject(CCString::create(element.key));
            container->addChild(button);
            
        } else if (element.type == UIElementType::Float) {
            auto checkbox = CCMenuItemToggler::createWithStandardSprites(
                this,
                menu_selector(UI::onCheckboxToggled),
                0.8f
            );
            checkbox->toggle(loadCheckboxState(element.key));
            checkbox->setUserObject(CCString::create(element.key));
            container->addChild(checkbox);
            
            auto plusBtn = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png"),
                this,
                menu_selector(UI::onFloatButtonPressed)
            );
            plusBtn->setScale(0.6f);
            plusBtn->setUserObject(CCString::create(element.key));
            container->addChild(plusBtn);
        }
        
        auto label = CCLabelBMFont::create(element.name.c_str(), "bigFont.fnt");
        label->setScale(0.6f);
        container->addChild(label);
        
        container->updateLayout();
        m_contentMenu->addChild(container);
    }
    
    m_contentMenu->setContentSize({350.f, (float)(elements.size() * 40)});
    m_contentMenu->updateLayout();
    
    m_scrollView->addChild(m_contentMenu);
}

void UI::onButtonPressed(CCObject* sender) {
    auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto keyStr = static_cast<CCString*>(button->getUserObject());
    std::string key = keyStr->getCString();
    
    if (key == "Credits1") {
         FLAlertLayer::create("Credits", "Created by: Kai552", "OK")->show();
     } else if (key == "Credits2") {
         FLAlertLayer::create("Thanks", "Special thanks to Speedyfriend67", "OK")->show();
     } else if (key == "Credits3") {
         FLAlertLayer::create("Version", "Version 1.0.0", "OK")->show();
     }
 }

void UI::onFloatButtonPressed(CCObject* sender) {
    auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto keyStr = static_cast<CCString*>(button->getUserObject());
    std::string key = keyStr->getCString();
    
    auto popup = FloatConfigPopup::create(key);
    popup->show();
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
    (void)Mod::get()->saveData();
}
