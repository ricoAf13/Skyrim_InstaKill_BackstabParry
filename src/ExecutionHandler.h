#pragma once
#include "Data.h"
#include <random>
#include <shared_mutex>
#include <mutex>

class ExecutionHandler {
public:
    static ExecutionHandler* GetSingleton() {
        static ExecutionHandler singleton;
        return &singleton;
    }

    bool AttemptExecute(RE::Actor* a_executor, RE::Actor* a_victim);

private:
    ExecutionHandler() = default;

    void ExecuteHumanoid(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE a_weaponType);
    void ExecuteDraugr(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteFalmer(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteSpider(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteGargoyle(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteGiant(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteBear(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteSabreCat(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteWolf(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteTroll(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteHagraven(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteSpriggan(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteBoar(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteRiekling(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteAshHopper(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteSteamCenturion(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteDwarvenBallista(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteChaurusFlyer(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteLurker(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);
    void ExecuteDragon(RE::Actor* a_executor, RE::Actor* a_victim, RE::WEAPON_TYPE weaponType);

    void QueueExecutionIdle(RE::Actor* a_executor, RE::Actor* a_victim, const Data::KM& idles);
    
    // Helpers
    bool IsHumanoid(RE::Actor* a_actor);
    bool IsBackFacing(RE::Actor* a_target, RE::Actor* a_attacker);
    bool IsDualWielding(RE::Actor* a_actor);
    RE::TESObjectWEAP* GetWieldingWeapon(RE::Actor* a_actor);
};
