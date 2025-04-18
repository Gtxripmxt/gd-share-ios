#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/EditorScene.hpp>
#include <Geode/ui/TextInput.hpp>
#include <fstream>
#include <sstream>

using namespace geode;

class $modify(MenuLayerExportImport, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        auto exportBtn = CCMenuItemFont::create("Export Current Level", [](auto) {
            auto editorLayer = GameManager::sharedState()->getEditorLayer();
            if (!editorLayer || !editorLayer->m_level) {
                FLAlertLayer::create("Error", "No level loaded in editor", "OK")->show();
                return;
            }

            auto level = editorLayer->m_level;
            std::string gmd = level->getSaveString();
            std::string name = level->m_levelName;

            std::ofstream out(FileUtils::sharedFileUtils()->getWritablePath() + name + ".gmd");
            out << gmd;
            out.close();

            FLAlertLayer::create("Success", "Level exported!", "OK")->show();
        });
        exportBtn->setPosition({190, 120});
        m_buttonMenu->addChild(exportBtn);

        auto importBtn = CCMenuItemFont::create("Import Level from File", [](auto) {
            auto popup = TextInputPopup::create(
                "Import GMD",
                "Enter filename (e.g. level.gmd)",
                "Import",
                [](const std::string& filename) {
                    std::ifstream in(FileUtils::sharedFileUtils()->getWritablePath() + filename);
                    if (!in) {
                        FLAlertLayer::create("Error", "File not found", "OK")->show();
                        return;
                    }

                    std::stringstream buffer;
                    buffer << in.rdbuf();

                    auto level = G
