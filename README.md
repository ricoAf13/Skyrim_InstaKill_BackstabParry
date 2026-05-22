# Backstab and Parry (SKSE)

An attempt to re-create Backstab and Parry mod in an SKSE plugin

This plugin was entirely made by Gemini AI. The reason i do this so that i can at least get a cool killmove finisher when Skyrim decides to not doing it, even with the help from Violens - Killmove mod.

I told AI to take Critical Hit - Backstab and Parry in Skyrim Special Edition (https://www.nexusmods.com/skyrimspecialedition/mods/17400) as a reference. And Valhalla Combat for how it should do/trigger a kill move.

## What this mod do
- Backstab an enemy from behind (within a 120-degree rear cone) will trigger a vanilla backstab killmove animation.
- Power bash an enemy when they winding up their attack would cause stagger and a vulnerable state to them. Doing a follow-up attack would trigger random vanilla killmove animation, including decapitation.

Note that the killmove animation would instantly kill the target, even if their health point has an absurd amount. So, it won't work on Essential Flagged target.

## How It Works (SKSE/C++ Implementation) | Summarized by AI
This mod completely avoids Papyrus virtual machine polling in favor of a native C++ event-driven architecture. 
1. **Event Hooking**: The plugin hooks `BSAnimationGraphEvent` for both the Player and NPCs. It maps specific string events (like `weaponSwing`, `AttackStop`, and `bashStart`) to track exactly when an NPC is vulnerable and when the player is executing an attack.
2. **Combat Interception**: We utilize a `RE::BSTEventSink<RE::TESHitEvent>` to detect when the player's bash connects with an enemy during their mapped parry window. 
3. **Engine-Level Paired Animations**: Executions are triggered using Skyrim's native `Offset::playPairedIdle` engine function. Thanks to Valhalla combat.

## Requirements
To use this mod, you will need the following installed:
1. **Skyrim Script Extender (SKSE64)**: https://skse.silverlock.org/ / https://www.nexusmods.com/skyrimspecialedition/mods/30379
2. **Address Library for SKSE Plugins**: https://www.nexusmods.com/skyrimspecialedition/mods/32444
3. **Valhalla Combat**: https://www.nexusmods.com/skyrimspecialedition/mods/64741 *(Required for the ValhallaCombat.esp)*

## Compatibility
- Supported Versions: **Skyrim Special Edition (SE)** and **Anniversary Edition (AE)**.
- **Not Supported**: Skyrim VR is **NOT** supported. Why would you need paired kill move animations in VR anyway?
