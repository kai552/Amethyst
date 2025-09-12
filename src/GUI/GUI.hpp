#pragma once

#include <Geode/Geode.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include "../Client/Client.hpp"
using namespace geode::prelude;

enum class UIElementType {
    Checkbox,
    Button,
    Float
};

struct UIElement {
    std::string name;
    UIElementType type;
    std::string key;
};

class FloatConfigPopup : public geode::Popup<> {
protected:
    std::string m_settingKey;
    CCTextInputNode* m_input;
    
    bool setup(const std::string& settingKey) override;
    void onSave(CCObject* sender);
    void onCancel(CCObject* sender);
    
public:
    static FloatConfigPopup* create(const std::string& settingKey);
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
    void onCategoryButtonPressed(CCObject* sender);
    void onCheckboxToggled(CCObject* sender);
    void onButtonPressed(CCObject* sender);
    void onFloatButtonPressed(CCObject* sender);
    void setupCategoryContent(const std::string& category);
    void clearContent();
    
    void saveCheckboxState(const std::string& key, bool value);
    bool loadCheckboxState(const std::string& key, bool defaultValue = false);
    void saveAllStates();
    std::vector<UIElement> getCategoryElements(const std::string& categoryStr);
    
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
