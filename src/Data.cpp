#include "Data.h"
#include "INIParser.h"

void Data::LoadData() {
    SKSE::log::info("Loading data from game...");
    auto data = RE::TESDataHandler::GetSingleton();
    if (!data) {
        SKSE::log::critical("Error: TESDataHandler not found.");
        return;
    }
    
    LoadIdle(data);
    LoadExecutableRace(data);
    
    SKSE::log::info("Data fetched.");
}

bool Data::ParseFormString(const std::string& str, std::string& outPlugin, RE::FormID& outFormID) {
    auto pos = str.find('|');
    if (pos == std::string::npos) return false;
    
    outPlugin = str.substr(0, pos);
    std::string formStr = str.substr(pos + 1);
    
    try {
        outFormID = std::stoul(formStr, nullptr, 0);
        return true;
    } catch (...) {
        return false;
    }
}

void Data::LoadIdleSection(RE::TESDataHandler* data, KM* idleContainer, const std::string& section) {
    auto values = INIParser::GetValues(kmFilePath, section);
    int loaded = 0;
    for (const auto& val : values) {
        std::string plugin;
        RE::FormID form;
        if (ParseFormString(val, plugin, form)) {
            auto idle = data->LookupForm<RE::TESIdleForm>(form, plugin);
            if (idle) {
                idleContainer->push_back(idle);
                loaded++;
            }
        }
    }
    if (loaded > 0) {
        SKSE::log::info("Loaded {} idles from section {}.", loaded, section);
    }
}

void Data::LoadIdle(RE::TESDataHandler* data) {
    SKSE::log::info("Loading idles...");
    LoadIdleSection(data, &KM_Humanoid_H2H, "Humanoid-Unarmed");
    LoadIdleSection(data, &KM_Humanoid_Dagger, "Humanoid-Dagger");
    LoadIdleSection(data, &KM_Humanoid_Sword, "Humanoid-Sword");
    LoadIdleSection(data, &KM_Humanoid_Axe, "Humanoid-Axe");
    LoadIdleSection(data, &KM_Humanoid_Mace, "Humanoid-Mace");
    LoadIdleSection(data, &KM_Humanoid_GreatSword, "Humanoid-GreatSword");
    LoadIdleSection(data, &KM_Humanoid_2hw, "Humanoid-2HW");
    LoadIdleSection(data, &KM_Humanoid_DW, "Humanoid-DW");

    LoadIdleSection(data, &KM_Humanoid_1hm_Back, "Humanoid-1HM-Back");
    LoadIdleSection(data, &KM_Humanoid_2hm_Back, "Humanoid-2HM-Back");
    LoadIdleSection(data, &KM_Humanoid_2hw_Back, "Humanoid-2HW-Back");
    LoadIdleSection(data, &KM_Humanoid_H2H_Back, "Humanoid-Unarmed-Back");

    LoadIdleSection(data, &KM_Undead_1hm, "Undead-1HM");
    LoadIdleSection(data, &KM_Undead_2hm, "Undead-2HM");
    LoadIdleSection(data, &KM_Undead_2hw, "Undead-2HW");

    LoadIdleSection(data, &KM_Falmer_1hm, "Falmer-1HM");
    LoadIdleSection(data, &KM_Falmer_2hm, "Falmer-2HM");
    LoadIdleSection(data, &KM_Falmer_2hw, "Falmer-2HW");

    LoadIdleSection(data, &KM_Spider_1hm, "Spider-1HM");
    LoadIdleSection(data, &KM_Spider_2hm, "Spider-2HM");
    LoadIdleSection(data, &KM_Spider_2hw, "Spider-2HW");

    LoadIdleSection(data, &KM_Gargoyle_1hm, "Gargoyle-1HM");
    LoadIdleSection(data, &KM_Gargoyle_2hm, "Gargoyle-2HM");
    LoadIdleSection(data, &KM_Gargoyle_2hw, "Gargoyle-2HW");

    LoadIdleSection(data, &KM_Giant_1hm, "Giant-1HM");
    LoadIdleSection(data, &KM_Giant_2hm, "Giant-2HM");
    LoadIdleSection(data, &KM_Giant_2hw, "Giant-2HW");

    LoadIdleSection(data, &KM_Bear_1hm, "Bear-1HM");
    LoadIdleSection(data, &KM_Bear_2hm, "Bear-2HM");
    LoadIdleSection(data, &KM_Bear_2hw, "Bear-2HW");

    LoadIdleSection(data, &KM_SabreCat_1hm, "SabreCat-1HM");
    LoadIdleSection(data, &KM_SabreCat_2hm, "SabreCat-2HM");
    LoadIdleSection(data, &KM_SabreCat_2hw, "SabreCat-2HW");

    LoadIdleSection(data, &KM_Wolf_1hm, "Wolf-1HM");
    LoadIdleSection(data, &KM_Wolf_2hm, "Wolf-2HM");
    LoadIdleSection(data, &KM_Wolf_2hw, "Wolf-2HW");

    LoadIdleSection(data, &KM_Troll_1hm, "Troll-1HM");
    LoadIdleSection(data, &KM_Troll_2hm, "Troll-2HM");
    LoadIdleSection(data, &KM_Troll_2hw, "Troll-2HW");

    LoadIdleSection(data, &KM_Hagraven_1hm, "Hagraven-1HM");
    LoadIdleSection(data, &KM_Hagraven_2hm, "Hagraven-2HM");
    LoadIdleSection(data, &KM_Hagraven_2hw, "Hagraven-2HW");

    LoadIdleSection(data, &KM_Spriggan_1hm, "Spriggan-1HM");
    LoadIdleSection(data, &KM_Spriggan_2hm, "Spriggan-2HM");
    LoadIdleSection(data, &KM_Spriggan_2hw, "Spriggan-2HW");

    LoadIdleSection(data, &KM_Boar_1hm, "Boar-1HM");
    LoadIdleSection(data, &KM_Boar_2hm, "Boar-2HM");
    LoadIdleSection(data, &KM_Boar_2hw, "Boar-2HW");

    LoadIdleSection(data, &KM_Riekling_1hm, "Riekling-1HM");
    LoadIdleSection(data, &KM_Riekling_2hm, "Riekling-2HM");
    LoadIdleSection(data, &KM_Riekling_2hw, "Riekling-2HW");

    LoadIdleSection(data, &KM_AshHopper_1hm, "AshHopper-1HM");
    LoadIdleSection(data, &KM_AshHopper_2hm, "AshHopper-2HM");
    LoadIdleSection(data, &KM_AshHopper_2hw, "AshHopper-2HW");

    LoadIdleSection(data, &KM_Ballista_1hm, "DwarvenBallista-1HM");
    LoadIdleSection(data, &KM_Ballista_2hm, "DwarvenBallista-2HM");
    LoadIdleSection(data, &KM_Ballista_2hw, "DwarvenBallista-2HW");

    LoadIdleSection(data, &KM_Centurion_1hm, "SteamCenturion-1HM");
    LoadIdleSection(data, &KM_Centurion_2hm, "SteamCenturion-2HM");
    LoadIdleSection(data, &KM_Centurion_2hw, "SteamCenturion-2HW");

    LoadIdleSection(data, &KM_ChaurusFlyer_1hm, "ChaurusFlyer-1HM");
    LoadIdleSection(data, &KM_ChaurusFlyer_2hm, "ChaurusFlyer-2HM");
    LoadIdleSection(data, &KM_ChaurusFlyer_2hw, "ChaurusFlyer-2HW");

    LoadIdleSection(data, &KM_Lurker_1hm, "Lurker-1HM");
    LoadIdleSection(data, &KM_Lurker_2hm, "Lurker-2HM");
    LoadIdleSection(data, &KM_Lurker_2hw, "Lurker-2HW");

    LoadIdleSection(data, &KM_Dragon_1hm, "Dragon-1HM");
    LoadIdleSection(data, &KM_Dragon_2hm, "Dragon-2HM");
    LoadIdleSection(data, &KM_Dragon_2hw, "Dragon-2HW");
}

void Data::LoadRaceSection(RE::TESDataHandler* data, RaceCategory raceType, const std::string& iniPath, const std::string& section) {
    auto values = INIParser::GetValues(iniPath, section);
    int loaded = 0;
    for (const auto& val : values) {
        std::string plugin;
        RE::FormID form;
        if (ParseFormString(val, plugin, form)) {
            auto race = data->LookupForm<RE::TESRace>(form, plugin);
            if (race) {
                raceMapping.emplace(race, raceType);
                loaded++;
            }
        }
    }
    if (loaded > 0) {
        SKSE::log::info("Loaded {} races from section {}.", loaded, section);
    }
}

void Data::LoadExecutableRaceIni(RE::TESDataHandler* data, const std::string& iniPath) {
    LoadRaceSection(data, RaceCategory::Humanoid, iniPath, "Humanoid");
    LoadRaceSection(data, RaceCategory::Undead, iniPath, "Undead");
    LoadRaceSection(data, RaceCategory::Falmer, iniPath, "Falmer");
    LoadRaceSection(data, RaceCategory::Spider, iniPath, "Spider");
    LoadRaceSection(data, RaceCategory::Gargoyle, iniPath, "Gargoyle");
    LoadRaceSection(data, RaceCategory::Giant, iniPath, "Giant");
    LoadRaceSection(data, RaceCategory::Bear, iniPath, "Bear");
    LoadRaceSection(data, RaceCategory::SabreCat, iniPath, "SabreCat");
    LoadRaceSection(data, RaceCategory::Wolf, iniPath, "Wolf");
    LoadRaceSection(data, RaceCategory::Troll, iniPath, "Troll");
    LoadRaceSection(data, RaceCategory::Hagraven, iniPath, "Hagraven");
    LoadRaceSection(data, RaceCategory::Spriggan, iniPath, "Spriggan");
    LoadRaceSection(data, RaceCategory::Boar, iniPath, "Boar");
    LoadRaceSection(data, RaceCategory::Riekling, iniPath, "Riekling");
    LoadRaceSection(data, RaceCategory::AshHopper, iniPath, "AshHopper");
    LoadRaceSection(data, RaceCategory::SteamCenturion, iniPath, "SteamCenturion");
    LoadRaceSection(data, RaceCategory::DwarvenBallista, iniPath, "DwarvenBallista");
    LoadRaceSection(data, RaceCategory::ChaurusFlyer, iniPath, "ChaurusFlyer");
    LoadRaceSection(data, RaceCategory::Lurker, iniPath, "Lurker");
    LoadRaceSection(data, RaceCategory::Dragon, iniPath, "Dragon");
}

void Data::LoadExecutableRace(RE::TESDataHandler* data) {
    if (!std::filesystem::exists(kmRaceDir)) return;
    for (const auto& entry : std::filesystem::directory_iterator(kmRaceDir)) {
        if (entry.path().extension() == ".ini") {
            LoadExecutableRaceIni(data, entry.path().string());
        }
    }
}

bool Data::IsRaceType(RE::Actor* a_actor, RaceCategory a_category) {
    if (!a_actor || !a_actor->GetRace()) return false;
    auto it = raceMapping.find(a_actor->GetRace());
    if (it != raceMapping.end()) {
        return it->second == a_category;
    }
    return false;
}
