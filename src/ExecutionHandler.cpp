#include "ExecutionHandler.h"
#include <thread>
#include <chrono>

namespace Offset {
    inline bool playPairedIdle(RE::AIProcess* proc, RE::Actor* attacker, RE::DEFAULT_OBJECT smth, RE::TESIdleForm* idle, bool a5, bool a6, RE::TESObjectREFR* target) {
        using func_t = decltype(&playPairedIdle);
        REL::Relocation<func_t> func{ RELOCATION_ID(38290, 39256) };
        return func(proc, attacker, smth, idle, a5, a6, target);
    }
}

bool ExecutionHandler::AttemptExecute(RE::Actor* a_executor, RE::Actor* a_victim) {
    if (!a_executor || !a_victim) return false;
    if (a_executor->IsDead() || a_victim->IsDead() || a_executor->IsInKillMove() || a_victim->IsInKillMove()) return false;
    if (a_executor->IsOnMount() || a_victim->IsOnMount()) return false;
    if (a_victim->IsPlayerRef() || a_victim->IsEssential()) return false;
    
    auto victimRace = a_victim->GetRace();
    if (!victimRace) return false;
    
    auto data = Data::GetSingleton();
    auto it = data->raceMapping.find(victimRace);
    if (it == data->raceMapping.end()) {
        SKSE::log::info("Target race is not mapped for execution.");
        return false;
    }
    
    Data::RaceCategory victimRaceType = it->second;
    RE::WEAPON_TYPE weaponType = RE::WEAPON_TYPE::kHandToHandMelee;
    
    auto weapon = GetWieldingWeapon(a_executor);
    if (weapon) {
        weaponType = weapon->GetWeaponType();
    }
    
    if (weaponType == RE::WEAPON_TYPE::kBow || weaponType == RE::WEAPON_TYPE::kCrossbow || weaponType == RE::WEAPON_TYPE::kStaff) {
        return false;
    }
    
    switch (victimRaceType) {
        case Data::RaceCategory::Humanoid: ExecuteHumanoid(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::Undead: ExecuteDraugr(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::Falmer: ExecuteFalmer(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::Spider: ExecuteSpider(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::Gargoyle: ExecuteGargoyle(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::Giant: ExecuteGiant(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::Bear: ExecuteBear(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::SabreCat: ExecuteSabreCat(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::Wolf: ExecuteWolf(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::Troll: ExecuteTroll(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::Hagraven: ExecuteHagraven(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::Spriggan: ExecuteSpriggan(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::Boar: ExecuteBoar(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::Riekling: ExecuteRiekling(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::AshHopper: ExecuteAshHopper(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::SteamCenturion: ExecuteSteamCenturion(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::DwarvenBallista: ExecuteDwarvenBallista(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::ChaurusFlyer: ExecuteChaurusFlyer(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::Lurker: ExecuteLurker(a_executor, a_victim, weaponType); break;
        case Data::RaceCategory::Dragon: ExecuteDragon(a_executor, a_victim, weaponType); break;
        default: return false;
    }
    return true;
}

void ExecutionHandler::QueueExecutionIdle(RE::Actor* a_executor, RE::Actor* a_victim, const Data::KM& idles) {
    if (idles.empty()) {
        SKSE::log::info("No idles available for this race/weapon combination.");
        return;
    }
    
    // Select a random idle
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, idles.size() - 1);
    RE::TESIdleForm* selectedIdle = idles[dist(gen)];
    
    // Queue SKSE Task to play the idle
    auto task = SKSE::GetTaskInterface();
    if (task) {
        task->AddTask([a_executor, a_victim, selectedIdle]() {
            if (a_executor && a_victim && selectedIdle) {
                Offset::playPairedIdle(a_executor->GetActorRuntimeData().currentProcess, a_executor, RE::DEFAULT_OBJECT::kActionIdle, selectedIdle, true, false, a_victim);
            }
        });
    }
}

// ---------------- Helpers ----------------

bool ExecutionHandler::IsHumanoid(RE::Actor* a_actor) {
    return Data::GetSingleton()->IsRaceType(a_actor, Data::RaceCategory::Humanoid);
}

bool ExecutionHandler::IsBackFacing(RE::Actor* a_target, RE::Actor* a_attacker) {
    if (!a_target || !a_attacker) return false;
    float heading = a_target->GetHeadingAngle(a_attacker->GetPosition(), false);
    return (heading > 120.0f || heading < -120.0f);
}

bool ExecutionHandler::IsDualWielding(RE::Actor* a_actor) {
    if (!a_actor) return false;
    auto left = a_actor->GetEquippedObject(true);
    auto right = a_actor->GetEquippedObject(false);
    if (left && right) {
        return left->IsWeapon() && right->IsWeapon();
    }
    return false;
}

RE::TESObjectWEAP* ExecutionHandler::GetWieldingWeapon(RE::Actor* a_actor) {
    if (!a_actor) return nullptr;
    auto rightObj = a_actor->GetEquippedObject(false);
    if (rightObj && rightObj->IsWeapon()) {
        return rightObj->As<RE::TESObjectWEAP>();
    }
    return nullptr;
}

// ---------------- Race Specific Dispatchers ----------------

void ExecutionHandler::ExecuteHumanoid(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE a_weaponType) {
    auto data = Data::GetSingleton();
    if (IsDualWielding(a_executor)) {
        QueueExecutionIdle(a_executor, a_victim, data->KM_Humanoid_DW);
    }
    else if (IsBackFacing(a_victim, a_executor)) {
        switch (a_weaponType) {
            case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Humanoid_2hw_Back); break;
            case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Humanoid_2hm_Back); break;
            case RE::WEAPON_TYPE::kHandToHandMelee: QueueExecutionIdle(a_executor, a_victim, data->KM_Humanoid_H2H_Back); break;
            default: QueueExecutionIdle(a_executor, a_victim, data->KM_Humanoid_1hm_Back); break;
        }
    }
    else {
        switch (a_weaponType) {
            case RE::WEAPON_TYPE::kOneHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Humanoid_Sword); break;
            case RE::WEAPON_TYPE::kOneHandDagger: QueueExecutionIdle(a_executor, a_victim, data->KM_Humanoid_Dagger); break;
            case RE::WEAPON_TYPE::kOneHandAxe:  QueueExecutionIdle(a_executor, a_victim, data->KM_Humanoid_Axe); break;
            case RE::WEAPON_TYPE::kOneHandMace:  QueueExecutionIdle(a_executor, a_victim, data->KM_Humanoid_Mace); break;
            case RE::WEAPON_TYPE::kTwoHandAxe:  QueueExecutionIdle(a_executor, a_victim, data->KM_Humanoid_2hw); break;
            case RE::WEAPON_TYPE::kTwoHandSword:  QueueExecutionIdle(a_executor, a_victim, data->KM_Humanoid_GreatSword); break;
            case RE::WEAPON_TYPE::kHandToHandMelee:  QueueExecutionIdle(a_executor, a_victim, data->KM_Humanoid_H2H); break;
            default: break;
        }
    }
}

void ExecutionHandler::ExecuteDraugr(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Undead_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Undead_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Undead_1hm); break;
    }
}

void ExecutionHandler::ExecuteFalmer(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Falmer_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Falmer_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Falmer_1hm); break;
    }
}

void ExecutionHandler::ExecuteSpider(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Spider_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Spider_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Spider_1hm); break;
    }
}

void ExecutionHandler::ExecuteGargoyle(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Gargoyle_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Gargoyle_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Gargoyle_1hm); break;
    }
}

void ExecutionHandler::ExecuteGiant(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Giant_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Giant_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Giant_1hm); break;
    }
}

void ExecutionHandler::ExecuteBear(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Bear_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Bear_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Bear_1hm); break;
    }
}

void ExecutionHandler::ExecuteSabreCat(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_SabreCat_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_SabreCat_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_SabreCat_1hm); break;
    }
}

void ExecutionHandler::ExecuteWolf(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Wolf_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Wolf_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Wolf_1hm); break;
    }
}

void ExecutionHandler::ExecuteTroll(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Troll_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Troll_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Troll_1hm); break;
    }
}

void ExecutionHandler::ExecuteHagraven(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Hagraven_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Hagraven_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Hagraven_1hm); break;
    }
}

void ExecutionHandler::ExecuteSpriggan(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Spriggan_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Spriggan_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Spriggan_1hm); break;
    }
}

void ExecutionHandler::ExecuteBoar(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Boar_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Boar_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Boar_1hm); break;
    }
}

void ExecutionHandler::ExecuteRiekling(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Riekling_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Riekling_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Riekling_1hm); break;
    }
}

void ExecutionHandler::ExecuteAshHopper(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_AshHopper_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_AshHopper_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_AshHopper_1hm); break;
    }
}

void ExecutionHandler::ExecuteSteamCenturion(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Centurion_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Centurion_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Centurion_1hm); break;
    }
}

void ExecutionHandler::ExecuteDwarvenBallista(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Ballista_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Ballista_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Ballista_1hm); break;
    }
}

void ExecutionHandler::ExecuteChaurusFlyer(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_ChaurusFlyer_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_ChaurusFlyer_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_ChaurusFlyer_1hm); break;
    }
}

void ExecutionHandler::ExecuteLurker(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Lurker_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Lurker_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Lurker_1hm); break;
    }
}

void ExecutionHandler::ExecuteDragon(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType) {
    auto data = Data::GetSingleton();
    switch (weaponType) {
        case RE::WEAPON_TYPE::kTwoHandAxe: QueueExecutionIdle(a_executor, a_victim, data->KM_Dragon_2hw); break;
        case RE::WEAPON_TYPE::kTwoHandSword: QueueExecutionIdle(a_executor, a_victim, data->KM_Dragon_2hm); break;
        case RE::WEAPON_TYPE::kHandToHandMelee: break;
        default: QueueExecutionIdle(a_executor, a_victim, data->KM_Dragon_1hm); break;
    }
}
