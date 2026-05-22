#include "BackstabParry.h"
#include "ExecutionHandler.h"
#include <RE/Skyrim.h>
#include <cmath>

#undef PlaySound

void BackstabParry::RegisterNPCParryWindow(RE::Actor* a_actor) {
    if (!a_actor) return;
    std::unique_lock lock(_parryMutex);
    _npcsInParryWindow.insert(a_actor->GetFormID());
}

void BackstabParry::UnregisterNPCParryWindow(RE::Actor* a_actor) {
    if (!a_actor) return;
    std::unique_lock lock(_parryMutex);
    _npcsInParryWindow.erase(a_actor->GetFormID());
}

bool BackstabParry::IsInParryWindow(RE::Actor* a_actor) {
    if (!a_actor) return false;
    std::shared_lock lock(_parryMutex);
    return _npcsInParryWindow.find(a_actor->GetFormID()) != _npcsInParryWindow.end();
}

void BackstabParry::SetVulnerable(RE::Actor* a_target) {
    if (!a_target) return;
    std::unique_lock lock(_vulnerableMutex);
    vulnerableEnemies[a_target->GetFormID()].expirationTime = std::chrono::steady_clock::now() + std::chrono::seconds(2);
}

bool BackstabParry::IsVulnerable(RE::Actor* a_target) {
    if (!a_target) return false;
    std::shared_lock lock(_vulnerableMutex);
    auto it = vulnerableEnemies.find(a_target->GetFormID());
    if (it != vulnerableEnemies.end()) {
        if (std::chrono::steady_clock::now() < it->second.expirationTime) {
            return true;
        }
    }
    return false;
}

void BackstabParry::TryParry(RE::Actor* a_attacker, RE::Actor* a_target) {
    if (!a_attacker || !a_target) return;
    
    // Only player can parry for now
    if (!a_attacker->IsPlayerRef()) return;
    
    // Play parry sound (using block shield sound as placeholder)
    RE::PlaySound("WPNSwingUnarmed"); // Need proper sound later
    
    // Stagger the target
    a_target->SetGraphVariableFloat("staggerMagnitude", 1.0f);
    a_target->NotifyAnimationGraph("staggerStart");
    
    // Mark them as vulnerable for execution
    SetVulnerable(a_target);

}

void BackstabParry::TryBackstab(RE::Actor* a_attacker) {
    if (!a_attacker) return;
    
    // Find the best target by scanning nearby actors
    RE::Actor* target = nullptr;
    float closestDistSq = 250.0f * 250.0f; // Max backstab distance
    
    auto processLists = RE::ProcessLists::GetSingleton();
    if (processLists) {
        for (auto& handle : processLists->highActorHandles) {
            if (auto actorPtr = handle.get()) {
                RE::Actor* actor = actorPtr.get();
                if (!actor || actor == a_attacker || actor->IsDead() || actor->IsInKillMove()) continue;
                
                RE::NiPoint3 toAttacker = a_attacker->GetPosition() - actor->GetPosition();
                float distSq = toAttacker.x * toAttacker.x + toAttacker.y * toAttacker.y + toAttacker.z * toAttacker.z;
                
                if (distSq < closestDistSq) {
                    closestDistSq = distSq;
                    target = actor;
                }
            }
        }
    }
    
    if (!target) {

        return;
    }
    
    // First, check if the closest target is vulnerable to a parry execution
    if (IsVulnerable(target)) {
        bool success = ExecutionHandler::GetSingleton()->AttemptExecute(a_attacker, target);
        if (success) {

            return;
        }
    }
    
    // Check if attacker is behind the target (Backstab)
    // GetHeadingAngle returns degrees between the actor's forward vector and the point.
    // 0 = front, 180/-180 = directly behind.
    float angle = target->GetHeadingAngle(a_attacker->GetPosition(), false);
    
    // Check if the attacker is within a 120-degree cone behind the target (60 degrees each side)
    if (angle > 120.0f || angle < -120.0f) {
        // Success backstab
        RE::PlaySound("UICombatSneakAttack");
        
        // Trigger Backstab execution
        bool success = ExecutionHandler::GetSingleton()->AttemptExecute(a_attacker, target);
        if (success) {

        } else {

        }
    } else {

    }
}

void BackstabParry::TryBashParry(RE::Actor* a_attacker) {
    if (!a_attacker) return;
    
    // Find nearby attacking enemies
    auto processLists = RE::ProcessLists::GetSingleton();
    if (!processLists) return;
    
    for (auto& handle : processLists->highActorHandles) {
        if (auto actorPtr = handle.get()) {
            RE::Actor* enemy = actorPtr.get();
            if (!enemy || enemy == a_attacker || enemy->IsDead()) continue;
            
            // Distance check (250 units)
            RE::NiPoint3 toAttacker = a_attacker->GetPosition() - enemy->GetPosition();
            float distSq = toAttacker.x * toAttacker.x + toAttacker.y * toAttacker.y + toAttacker.z * toAttacker.z;
            
            if (distSq <= 250.0f * 250.0f) {
                // If they are inside their parry window, PARRY THEM!
                if (IsInParryWindow(enemy)) {

                    TryParry(a_attacker, enemy);
                    return; // Only parry one enemy at a time
                }
            }
        }
    }
    

}
