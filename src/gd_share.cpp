#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/EditorScene.hpp>
#include "cocos/platform/CCFileUtils.h"

using namespace geode;

$onMod(Loaded) {
    Mod::get()->addCustomMenuItem("Export Current Level", [] {
        auto editor = EditorScene::get();
        if (!editor) {
            log::warn("No active level editor found.");
            return;
        }

        auto level = editor->m_level;
        if (!level) {
            log::warn("No level data available.");
            return;
        }

        std::string levelData = level->getLevelData();
        std::string levelName = level->m_levelName;

        auto path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath() + levelName + ".gmd";
        std::ofstream outFile(path);
        if (!outFile) {
            log::error("Failed to create file at {}", path);
            return;
        }

        outFile << levelData;
        outFile.close();
        log::info("Level exported to {}", path);
    });

    Mod::get()->addCustomMenuItem("Import Level from File", [] {
        std::string fileName = "import.gmd";
        auto path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath() + fileName;

        std::ifstream inFile(path);
        if (!inFile) {
            log::error("Failed to open file at {}", path);
            return;
        }

        std::string data((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();

        auto level = GJGameLevel::create();
        level->setLevelData(data);
        level->m_levelName = "Imported Level";

        CCDirector::sharedDirector()->replaceScene(EditorScene::scene(level));
    });
}
