#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(CUTETrail, PlayerObject) {
    bool init(int playerFrame, int vehicleFrame, GJBaseGameLayer* layer, cocos2d::CCLayer* p4, bool p5) {
        if (!PlayerObject::init(playerFrame, vehicleFrame, layer, p4, p5)) return false;

        if (Mod::get()->getSettingValue<bool>("trail-enabled")) {
            this->applyTrailSettings();
        }
        return true;
    }

    void applyTrailSettings() {
        auto colorVal = Mod::get()->getSettingValue<cocos2d::ccColor4B>("trail-color");
        int opacity   = Mod::get()->getSettingValue<int64_t>("trail-opacity");
        int length    = Mod::get()->getSettingValue<int64_t>("trail-length");

        ccColor3B color = { colorVal.r, colorVal.g, colorVal.b };

        // m_regularTrail is the CCMotionStreak for the default trail
        if (m_regularTrail) {
            m_regularTrail->setColor(color);
            m_regularTrail->setOpacity(static_cast<GLubyte>(opacity));
            // Streak fade time loosely maps to visual trail length
            // Typical fade: 0.3–1.0s. We map length [4,30] → [0.15, 0.9]
            float fade = 0.15f + (length - 4) * (0.75f / 26.0f);
            m_regularTrail->setFade(fade);
        }
    }

    // Re-apply when the trail gets reset (e.g. after death)
    void resetObject(bool p0) {
        PlayerObject::resetObject(p0);
        if (Mod::get()->getSettingValue<bool>("trail-enabled")) {
            this->applyTrailSettings();
        }
    }
};
