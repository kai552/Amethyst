#ifndef GEODE_IS_IOS
#include "../GUI/GUI.hpp"
#include <Geode/modify/CCKeyboardDispatcher.hpp>
class $modify(AmethystUIHook, cocos2d::CCKeyboardDispatcher) {
    bool dispatchKeyboardMSG(cocos2d::enumKeyCodes key, bool isKeyDown, bool isKeyRepeat) {
        if (key == cocos2d::enumKeyCodes::KEY_Tab && isKeyDown) {
            UI::create()->show();
        }
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, isKeyDown, isKeyRepeat);
    }
};
#endif
