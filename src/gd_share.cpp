#include <Geode/Geode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/binding/LevelBrowserLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

$hook(LevelBrowserLayer::init)
bool LevelBrowserLayer::init(GJSearchObject* search) {
    if (!LevelBrowserLayer::init(search)) return false;

    if (search->m_searchType != SearchType::MyLevels)
        return true;

    auto sprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
    sprite->setScale(0.5f);

    auto btn = ccMenuItemSpriteExtra::create(
        sprite,
        this,
        [](CCObject*) {
            spdlog::info("Import");
        }
    );

    auto menu = CCMenu::create();
    menu->addChild(btn);
    menu->setPosition({ 50.f, 75.f });
    this->addChild(menu);

    return true;
}

// Hook for EditLevelLayer::init
$hook(EditLevelLayer::init)
bool EditLevelLayer::init(GJGameLevel* level) {
    if (!EditLevelLayer::init(level)) return false;

    auto sprite = CCSprite::createWithSpriteFrameName("GJ_shareBtn_001.png");
    sprite->setScale(0.5f);

    auto btn = ccMenuItemSpriteExtra::create(
        sprite,
        this,
        [level](CCObject*) {
            spdlog::info("Export {}", level->m_levelName);
        }
    );

    auto menu = CCMenu::create();
    menu->addChild(btn);
    menu->setPosition({ 50.f, 100.f });
    this->addChild(menu);

    return true;
}
