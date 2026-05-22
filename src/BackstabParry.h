#pragma once

#include <shared_mutex>
#include <unordered_set>
#include <unordered_map>
#include <chrono>

class BackstabParry {
public:
    static BackstabParry* GetSingleton() {
        static BackstabParry singleton;
        return &singleton;
    }

    void RegisterNPCParryWindow(RE::Actor* a_actor);
    void UnregisterNPCParryWindow(RE::Actor* a_actor);
    bool IsInParryWindow(RE::Actor* a_actor);

    void TryParry(RE::Actor* a_attacker, RE::Actor* a_target);
    void TryBackstab(RE::Actor* a_attacker);
    
    // Scans nearby enemies and parries them if they are in their attack window
    void TryBashParry(RE::Actor* a_attacker);

    void SetVulnerable(RE::Actor* a_target);
    bool IsVulnerable(RE::Actor* a_target);

private:
    BackstabParry() = default;
    ~BackstabParry() = default;
    
    struct VulnerableState {
        std::chrono::time_point<std::chrono::steady_clock> expirationTime;
    };
    std::unordered_map<RE::FormID, VulnerableState> vulnerableEnemies;
    std::shared_mutex _vulnerableMutex;

    std::shared_mutex _parryMutex;
    std::unordered_set<RE::FormID> _npcsInParryWindow;
};
