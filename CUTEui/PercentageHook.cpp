#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(CUTEPercentage, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (Mod::get()->getSettingValue<bool>("percentage-enabled")) {
            this->applyPercentageStyle();
        }
        return true;
    }

    void applyPercentageStyle() {
        if (!m_percentLabel) return;

        auto colorVal = Mod::get()->getSettingValue<cocos2d::ccColor4B>("percentage-color");
        ccColor3B color = { colorVal.r, colorVal.g, colorVal.b };

        m_percentLabel->setColor(color);
        m_percentLabel->setOpacity(230);

        // Slight scale-up for readability — feels cuter
        m_percentLabel->setScale(0.55f);

        // Soft drop shadow via a second label layered behind
        // We create a shadow copy once and position it offset
        auto shadowTag = 9901; // arbitrary tag to avoid duplicates
        if (!m_uiLayer->getChildByTag(shadowTag)) {
            auto shadow = CCLabelBMFont::create("0%", m_percentLabel->getFntFile().c_str());
            shadow->setTag(shadowTag);
            shadow->setColor({ 180, 120, 160 });
            shadow->setOpacity(80);
            shadow->setScale(m_percentLabel->getScale());
            shadow->setPosition(m_percentLabel->getPosition() + CCPoint(1.5f, -1.5f));
            shadow->setZOrder(m_percentLabel->getZOrder() - 1);
            m_uiLayer->addChild(shadow);
        }
    }

    void updateProgressbar() {
        PlayLayer::updateProgressbar();

        if (!Mod::get()->getSettingValue<bool>("percentage-enabled")) return;
        if (!m_percentLabel) return;

        // Keep shadow label synced with main label text & position
        if (auto shadow = typeinfo_cast<CCLabelBMFont*>(m_uiLayer->getChildByTag(9901))) {
            shadow->setString(m_percentLabel->getString());
            shadow->setPosition(m_percentLabel->getPosition() + CCPoint(1.5f, -1.5f));
        }
    }
};
