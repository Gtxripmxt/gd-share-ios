#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/GJBaseGameLayer.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>
#include <Geode/binding/CCFileUtils.hpp>
#include <cocos2d.h>
#include <Geode/ui/TextAlertPopup.hpp>

using namespace geode;

std::string getDocumentsPath() {
    return cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath();
}

void exportCurrentLevel() {
    auto editor = LevelEditorLayer::get();
    if (!editor) {
        FLAlertLayer::create("Error", "No level is currently open.", "OK")->show();
        return;
    }

    GJGameLevel* level = editor->m_level;

    std::string path = getDocumentsPath() + level->m_levelName + ".gmd";

    std::ofstream file(path);
    if (!file.is_open()) {
        FLAlertLayer::create("Error", "Could not open file for writing.", "OK")->show();
        return;
    }

    file << level->getSaveString();
    file.close();

    FLAlertLayer::create("Exported", ("Saved to:\n" + path).c_str(), "OK")->show();
}

void importLevelFromFile() {
    std::string path = getDocumentsPath() + "import.gmd";

    std::ifstream file(path);
    if (!file.is_open()) {
        FLAlertLayer::create("Error", "Could not find import.gmd in documents directory.", "OK")->show();
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string data = buffer.str();

    GJGameLevel* level = GJGameLevel::create();
    level->setSaveString(data);
    level->m_levelName = "Imported Level";

    auto scene = LevelEditorLayer::scene(level, false);
    cocos2d::CCDirector::sharedDirector()->replaceScene(scene);
}

$onMod(Loaded) {
    Mod::get()->addCustomMenuItem("Export Current Level", [] {
        exportCurrentLevel();
    });

    Mod::get()->addCustomMenuItem("Import Level from 'import.gmd'", [] {
        importLevelFromFile();
    });
}
