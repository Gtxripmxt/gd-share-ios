#include <Geode/Geode.hpp>
#include <Geode/modify/LevelTools.hpp>
#include <Geode/ui/TextAlertPopup.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>
#include <fstream>

using namespace geode::prelude;

std::string getWritablePath(const std::string& filename) {
    return CCFileUtils::sharedFileUtils()->getWritablePath() + filename;
}

void exportLevel(GJGameLevel* level) {
    if (!level) return;
    
    std::string levelData = level->m_levelString;
    std::string name = level->m_levelName;
    std::string filename = name + ".gmd";

    std::ofstream out(getWritablePath(filename));
    if (out) {
        out << levelData;
        out.close();
        FLAlertLayer::create("Exported!", "Level exported to Documents as " + filename, "OK")->show();
    } else {
        FLAlertLayer::create("Error", "Could not write to file!", "OK")->show();
    }
}

void importLevel(const std::string& filename) {
    std::ifstream in(getWritablePath(filename));
    if (!in) {
        FLAlertLayer::create("Error", "Could not open file!", "OK")->show();
        return;
    }

    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string levelString = buffer.str();

    auto level = GJGameLevel::create();
    level->m_levelString = levelString;
    level->m_levelName = "ImportedLevel";
    level->m_levelID = 0;
    level->m_levelDesc = "Imported using mod";

    LevelTools::addNewLevel(level);

    FLAlertLayer::create("Imported!", "Level imported successfully!", "OK")->show();
}

$onMod(Loaded) {
    Mod::get()->addCustomMenuItem("Export Current Level", [] {
        auto level = GameManager::sharedState()->getEditorLayer()->m_level;
        exportLevel(level);
    });

    Mod::get()->addCustomMenuItem("Import Level from file", [] {
        // This assumes there's a file named "import.gmd" in Documents
        importLevel("import.gmd");
    });
}
