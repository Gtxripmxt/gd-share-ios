#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/binding/LevelManager.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/ui/TextInput.hpp>
#include <fstream>

using namespace geode;

class $modify(MenuLayerExportImport, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        auto exportBtn = CCMenuItemFont::create("Export Current Level", [](auto) {
            auto level = GameManager::sharedState()->getEditorLayer()->m_level;
            if (!level) {
                FLAlertLayer::create("Error", "No level loaded", "OK")->show();
                return;
            }

            std::string gmd = level->getSaveString();
            std::string name = level->m_levelName;

            std::ofstream out(FileUtils::sharedFileUtils()->getWritablePath() + name + ".gmd");
            out << gmd;
            out.close();

            FLAlertLayer::create("Success", "Level exported to .gmd!", "Nice")->show();
        });
        exportBtn->setPosition({190, 120});
        m_buttonMenu->addChild(exportBtn);

        auto importBtn = CCMenuItemFont::create("Import Level from File", [](auto) {
            auto popup = TextInputPopup::create(
                "Import GMD",
                "Enter level name to import (e.g. testlevel.gmd)",
                "Import",
                [](const std::string& filename) {
                    std::ifstream in(FileUtils::sharedFileUtils()->getWritablePath() + filename);
                    if (!in) {
                        FLAlertLayer::create("Error", "File not found", "OK")->show();
                        return;
                    }

                    std::stringstream buffer;
                    buffer << in.rdbuf();

                    auto level = GJGameLevel::create();
                    level->setSaveString(buffer.str());
                    level->m_levelName = "ImportedLevel";

                    LevelManager::sharedState()->addCustomLevel(level);
                    GameManager::sharedState()->loadEditor(level);
                }
            );
            popup->show();
        });
