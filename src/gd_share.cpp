#include <Geode/Geode.hpp>
#include <Geode/modify/Modify.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>

using namespace geode::prelude;

// Button callback classes
class ImportButtonHandler : public cocos2d::CCObject {
public:
    void onImport(CCObject*) {
        log::info("Import pressed");
    }
};

class ExportButtonHandler : public cocos2d::CCObject {
public:
    GJGameLevel* m_level;

    ExportButtonHandler(GJGameLevel* level) : m_level(level) {}

    void onExport(CCObject*) {
        log::info("Export pressed for level: {}", m_level->m_levelName);
    }
};

// Hook LevelBrowserLayer::init
class $modify(LevelBrowserLayerHook, LevelBrowserLayer) {
    bool init(GJSearchObject* search) {
        if (!LevelBrowserLayer::init(search))
            return false;

        if (search->m_searchType != SearchType::MyLevels)
            return true;

        auto sprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        sprite->setScale(0.5f);

        auto handler = new ImportButtonHandler();
        handler->autorelease();

        auto btn = CCMenuItemSpriteExtra::create(
            sprite,
            handler,
            menu_selector(ImportButtonHandler::onImport)
        );

        auto menu = CCMenu::create();
        menu->addChild(btn);
        menu->setPosition({50.f, 75.f});
        this->addChild(menu);

        return true;
    }
};

// Hook EditLevelLayer::init
class $modify(EditLevelLayerHook, EditLevelLayer) {
    bool init(GJGameLevel* level) {
        if (!EditLevelLayer::init(level))
            return false;

        auto sprite = CCSprite::createWithSpriteFrameName("GJ_shareBtn_001.png");
        sprite->setScale(0.5f);

        auto handler = new ExportButtonHandler(level);
        handler->autorelease();

        auto btn = CCMenuItemSpriteExtra::create(
            sprite,
            handler,
            menu_selector(ExportButtonHandler::onExport)
        );

        auto menu = CCMenu::create();
        menu->addChild(btn);
        menu->setPosition({50.f, 100.f});
        this->addChild(menu);

        return true;
    }
};

