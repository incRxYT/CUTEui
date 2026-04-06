#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Helper: read a color setting as ccColor3B
static ccColor3B getColorSetting(const char* key) {
    auto val = Mod::get()->getSettingValue<cocos2d::ccColor4B>(key);
    return { val.r, val.g, val.b };
}

class $modify(CUTEProgressBar, PlayLayer) {
    struct Fields {
        CCSprite* barFill = nullptr;
        CCSprite* barBg = nullptr;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (!Mod::get()->getSettingValue<bool>("progress-bar-enabled")) return true;

        this->applyProgressBarSkin();
        return true;
    }

    void applyProgressBarSkin() {
        // The vanilla progress bar lives in m_progressBar (a SliderThumb/CCSprite tree)
        // We tint the bar fill and background using our settings colors.
        if (m_progressBar) {
            auto fillColor = getColorSetting("progress-bar-color");
            auto bgColor   = getColorSetting("progress-bar-bg-color");

            // m_progressBar children: [0] = bg, [1] = fill (vanilla layout)
            if (auto bg = typeinfo_cast<CCSprite*>(m_progressBar->getChildren()->objectAtIndex(0))) {
                bg->setColor(bgColor);
                bg->setOpacity(220);
            }
            if (m_progressBar->getChildrenCount() >= 2) {
                if (auto fill = typeinfo_cast<CCSprite*>(m_progressBar->getChildren()->objectAtIndex(1))) {
                    fill->setColor(fillColor);
                    fill->setOpacity(255);
                }
            }

            if (Mod::get()->getSettingValue<bool>("progress-bar-rounded")) {
                // Scale Y slightly for a pill shape feel
                m_progressBar->setScaleY(1.1f);
            }
        }
    }

    void updateProgressbar() {
        PlayLayer::updateProgressbar();
        // Re-apply tint each update so it survives vanilla overwrites
        if (Mod::get()->getSettingValue<bool>("progress-bar-enabled")) {
            auto fillColor = getColorSetting("progress-bar-color");
            if (m_progressBar && m_progressBar->getChildrenCount() >= 2) {
                if (auto fill = typeinfo_cast<CCSprite*>(m_progressBar->getChildren()->objectAtIndex(1))) {
                    fill->setColor(fillColor);
                }
            }
        }
    }
};
