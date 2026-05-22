#include "Hooks.h"
#include "BackstabParry.h"
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

namespace Hooks
{
    using EventResult = RE::BSEventNotifyControl;

    class AnimEventHook {
    public:
        static void Install() {
            REL::Relocation<uintptr_t> characterVtbl{ RE::VTABLE_Character[2] };
            _ProcessEvent_Character = characterVtbl.write_vfunc(0x1, ProcessEvent_Character);

            REL::Relocation<uintptr_t> playerVtbl{ RE::VTABLE_PlayerCharacter[2] };
            _ProcessEvent_Player = playerVtbl.write_vfunc(0x1, ProcessEvent_Player);
            
            SKSE::log::info("Installed Animation Event Hooks.");
        }

    private:
        static EventResult ProcessEvent_Character(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_source) {
            ProcessAnimEvent(a_event);
            return _ProcessEvent_Character(a_sink, a_event, a_source);
        }

        static EventResult ProcessEvent_Player(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_source) {
            ProcessAnimEvent(a_event);
            return _ProcessEvent_Player(a_sink, a_event, a_source);
        }

        static void ProcessAnimEvent(const RE::BSAnimationGraphEvent* a_event) {
            if (!a_event || !a_event->holder) return;
            
            auto actor = const_cast<RE::TESObjectREFR*>(a_event->holder)->As<RE::Actor>();
            if (!actor) return;
            
            std::string_view eventName = a_event->tag.data();
            
            if (actor->IsPlayerRef()) {
                if (eventName == "bashStart") {

                    BackstabParry::GetSingleton()->TryBashParry(actor);
                } else if (eventName == "PowerAttack_Start_end" || eventName == "weaponSwing" || eventName == "CastOKStart" || eventName == "MCO_WinOpen") {

                    BackstabParry::GetSingleton()->TryBackstab(actor);
                }
            } else {
                if (eventName == "weaponSwing" || eventName == "PowerAttack_Start_end" || eventName == "preHitFrame") {

                    BackstabParry::GetSingleton()->RegisterNPCParryWindow(actor);
                } else if (eventName == "AttackStop" || eventName == "MCO_WinClose" || 
                           eventName == "MCO_TransitionClose" || eventName == "BFCO_DIY_EndLoop") {
                    BackstabParry::GetSingleton()->UnregisterNPCParryWindow(actor);
                }
            }
        }

        static inline REL::Relocation<decltype(ProcessEvent_Character)> _ProcessEvent_Character;
        static inline REL::Relocation<decltype(ProcessEvent_Player)> _ProcessEvent_Player;
    };

    class HitEventSink : public RE::BSTEventSink<RE::TESHitEvent> {
    public:
        static HitEventSink* GetSingleton() {
            static HitEventSink singleton;
            return &singleton;
        }

        RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>*) override {
            if (!a_event || !a_event->target || !a_event->cause) return RE::BSEventNotifyControl::kContinue;

            auto attacker = a_event->cause->As<RE::Actor>();
            auto target = a_event->target->As<RE::Actor>();

            if (attacker && attacker->IsPlayerRef() && target) {
                if (BackstabParry::GetSingleton()->IsInParryWindow(target)) {

                    BackstabParry::GetSingleton()->TryParry(attacker, target);
                } else if (a_event->flags.any(RE::TESHitEvent::Flag::kBashAttack)) {

                }
            }

            return RE::BSEventNotifyControl::kContinue;
        }
    };

    void Install() {
        AnimEventHook::Install();

        auto scriptEventSource = RE::ScriptEventSourceHolder::GetSingleton();
        if (scriptEventSource) {
            scriptEventSource->AddEventSink(HitEventSink::GetSingleton());
            SKSE::log::info("Installed Hit Event Sink.");
        }
    }
}
