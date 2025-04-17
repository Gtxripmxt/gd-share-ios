#include <auby.hpp>

namespace {
bool $(LevelBrowserLayer::init)(LevelBrowserLayer* self, GJSearchObject* search) {
    $orig(self, search);

    if (search->m_searchType != SearchType::MyLevels)
        return true;

    auto sprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
    sprite->setScale(0.5);
    auto btn = ccMenuItemSpriteExtraWithCallback<[] {
        spdlog::info("Import");
    }>(sprite);

    auto menu = CCMenu::create();
    menu->addChild(btn);
    menu->setPosition({50, 75});

    self->addChild(menu);

    return true;
}

bool $(EditLevelLayer::init)(EditLevelLayer* self, GJGameLevel* level) {
    $orig(self, level);

    auto sprite = CCSprite::createWithSpriteFrameName("GJ_shareBtn_001.png");
    sprite->setScale(0.5);
    auto btn = ccMenuItemSpriteExtraWithCallback<[](GJGameLevel* level) {
        spdlog::info("Export {}", level->m_levelName);
    }>(sprite, level);

    auto menu = CCMenu::create();
    menu->addChild(btn);
    menu->setPosition({50, 100});

    self->addChild(menu);

    return true;
}
} // namespace
