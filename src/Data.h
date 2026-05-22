#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

class Data {
public:
    static Data* GetSingleton() {
        static Data singleton;
        return &singleton;
    }

    void LoadData();

    enum class RaceCategory {
        Humanoid = 0,
        Undead,
        Falmer,
        Spider,
        Gargoyle,
        Giant,
        Bear,
        SabreCat,
        Wolf,
        Troll,
        Hagraven,
        Spriggan,
        Boar,
        Riekling,
        AshHopper,
        SteamCenturion,
        DwarvenBallista,
        ChaurusFlyer,
        Lurker,
        Dragon
    };

    using KM = std::vector<RE::TESIdleForm*>;

    // Execution Idles
    KM KM_Humanoid_H2H, KM_Humanoid_Dagger, KM_Humanoid_Sword, KM_Humanoid_Axe, KM_Humanoid_Mace, KM_Humanoid_GreatSword, KM_Humanoid_2hw, KM_Humanoid_DW;
    KM KM_Humanoid_1hm_Back, KM_Humanoid_2hm_Back, KM_Humanoid_H2H_Back, KM_Humanoid_2hw_Back;
    KM KM_Falmer_1hm, KM_Falmer_2hm, KM_Falmer_2hw;
    KM KM_Undead_1hm, KM_Undead_2hm, KM_Undead_2hw;
    KM KM_Spider_1hm, KM_Spider_2hm, KM_Spider_2hw;
    KM KM_Gargoyle_1hm, KM_Gargoyle_2hm, KM_Gargoyle_2hw;
    KM KM_Giant_1hm, KM_Giant_2hm, KM_Giant_2hw;
    KM KM_Bear_1hm, KM_Bear_2hm, KM_Bear_2hw;
    KM KM_SabreCat_1hm, KM_SabreCat_2hm, KM_SabreCat_2hw;
    KM KM_Wolf_1hm, KM_Wolf_2hm, KM_Wolf_2hw;
    KM KM_Troll_1hm, KM_Troll_2hm, KM_Troll_2hw;
    KM KM_Hagraven_1hm, KM_Hagraven_2hm, KM_Hagraven_2hw;
    KM KM_Spriggan_1hm, KM_Spriggan_2hm, KM_Spriggan_2hw;
    KM KM_Boar_1hm, KM_Boar_2hm, KM_Boar_2hw;
    KM KM_Riekling_1hm, KM_Riekling_2hm, KM_Riekling_2hw;
    KM KM_AshHopper_1hm, KM_AshHopper_2hm, KM_AshHopper_2hw;
    KM KM_Centurion_1hm, KM_Centurion_2hm, KM_Centurion_2hw;
    KM KM_Ballista_1hm, KM_Ballista_2hm, KM_Ballista_2hw;
    KM KM_ChaurusFlyer_1hm, KM_ChaurusFlyer_2hm, KM_ChaurusFlyer_2hw;
    KM KM_Lurker_1hm, KM_Lurker_2hm, KM_Lurker_2hw;
    KM KM_Dragon_1hm, KM_Dragon_2hm, KM_Dragon_2hw;

    std::unordered_map<RE::TESRace*, RaceCategory> raceMapping;

    bool IsRaceType(RE::Actor* a_actor, RaceCategory a_category);

private:
    Data() = default;

    const std::string kmRaceDir = "Data/SKSE/Plugins/BackstabParry/RaceMapping";
    const std::string kmFilePath = "Data/SKSE/Plugins/BackstabParry/Killmoves.ini";

    void LoadIdleSection(RE::TESDataHandler* data, KM* idleContainer, const std::string& section);
    void LoadIdle(RE::TESDataHandler* data);

    void LoadRaceSection(RE::TESDataHandler* data, RaceCategory raceType, const std::string& iniPath, const std::string& section);
    void LoadExecutableRaceIni(RE::TESDataHandler* data, const std::string& iniPath);
    void LoadExecutableRace(RE::TESDataHandler* data);
    
    bool ParseFormString(const std::string& str, std::string& outPlugin, RE::FormID& outFormID);
};
