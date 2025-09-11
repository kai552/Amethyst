#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

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
