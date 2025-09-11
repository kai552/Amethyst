#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class UI : public geode::Popup<> {
protected:
    CCMenu* m_bottomButtonsMenu;

    bool setup() override;

    void addDefaultUICategoryButton(const std::string& category);
    void addLabel(const std::string& category);
    void onCategoryButtonPressed(CCObject* sender);
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
