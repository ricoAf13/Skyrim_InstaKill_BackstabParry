# Backstab and Parry (SKSE)

A lightweight, highly performant SKSE plugin for Skyrim Special Edition that introduces non-lethal Backstab and Parry execution mechanics. This mod builds on the foundation of Valhalla Combat's paired animation framework to deliver smooth, cinematic combat ripostes without relying on slow Papyrus scripts.

## Features
- **Parry Mechanics**: Intercept an enemy's attack by bashing them during their active attack frame (parry window). Successfully parrying will stagger the enemy, rendering them vulnerable to a frontal execution.
- **Backstab Mechanics**: Attacking an enemy from behind (within a 120-degree rear cone) will trigger a cinematic backstab execution.
- **Non-Lethal Executions**: Unlike vanilla kill moves, these paired execution animations apply a chunk of bonus damage but do not instantly kill the enemy, allowing for true combat ripostes that seamlessly return you to the fight if the enemy survives.

## How It Works (SKSE/C++ Implementation)
This mod completely avoids Papyrus virtual machine polling in favor of a native C++ event-driven architecture. 
1. **Event Hooking**: The plugin hooks `BSAnimationGraphEvent` for both the Player and NPCs. It maps specific string events (like `weaponSwing`, `AttackStop`, and `bashStart`) to track exactly when an NPC is vulnerable and when the player is executing an attack.
2. **Combat Interception**: We utilize a `RE::BSTEventSink<RE::TESHitEvent>` to detect when the player's bash connects with an enemy during their mapped parry window. 
3. **Engine-Level Paired Animations**: Executions are triggered using Skyrim's native `Offset::playPairedIdle` engine function. Depending on the attacker's weapon type and target's race/position, the C++ logic dynamically selects the correct Valhalla Nemesis paired animation event (e.g., `pa_Execution1hm` or `pa_BackAttack2hm`) and binds the executor and victim together perfectly in engine memory.

## Requirements
To use this mod, you will need the following installed:
1. **Skyrim Script Extender (SKSE64)**: https://skse.silverlock.org/ / https://www.nexusmods.com/skyrimspecialedition/mods/30379
2. **Address Library for SKSE Plugins**: https://www.nexusmods.com/skyrimspecialedition/mods/32444
3. **Valhalla Combat**: https://www.nexusmods.com/skyrimspecialedition/mods/64741 *(Required for the ValhallaCombat.esp and behavior files)*

## Compatibility
- Supported Versions: **Skyrim Special Edition (SE)** and **Anniversary Edition (AE)**.
- **Not Supported**: Skyrim VR is currently **NOT** supported.