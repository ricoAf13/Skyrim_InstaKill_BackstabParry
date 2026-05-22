#include "Hooks.h"
#include "Data.h"
#include <SKSE/SKSE.h>

static void SKSEMessageHandler(SKSE::MessagingInterface::Message* message)
{
    switch (message->type) {
    case SKSE::MessagingInterface::kDataLoaded:
        Data::GetSingleton()->LoadData();
        Hooks::Install();
        break;
    case SKSE::MessagingInterface::kPostLoadGame:
    case SKSE::MessagingInterface::kNewGame:
        RE::DebugNotification("Backstab and Parry Plugin Successfully Loaded!");
        break;
    }
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse);
    SKSE::AllocTrampoline(1 << 10);

    auto messaging = SKSE::GetMessagingInterface();
    if (messaging) {
        messaging->RegisterListener(SKSEMessageHandler);
    }

    SKSE::log::info("BackstabParry Plugin Loaded");

    return true;
}
