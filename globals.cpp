#pragma once

#include "pch.h"
#include "globals.h"
#include "util.h"

using namespace CG;


void gvar::cache_globals() {
    world = *UWorld::GWorld;
    if (!world) { all_initialized = false; return; };

    owning_game_instance = world->OwningGameInstance;
    if (!owning_game_instance) { all_initialized = false; return; };

    local_player = owning_game_instance->LocalPlayers[0];
    if (!local_player) { all_initialized = false; return; };

    local_player_controller = local_player->PlayerController;
    if (!local_player_controller) { all_initialized = false; return; };

    local_camera_manager = local_player_controller->PlayerCameraManager;
    if (!local_camera_manager) { all_initialized = false; return; };

    local_pawn = (ABP_PlayerCharacter_C*)local_player_controller->AcknowledgedPawn;
    if (!local_pawn) { all_initialized = false; return; };

    local_root_component = local_pawn->RootComponent;
    if (!local_root_component) { all_initialized = false; return; };

    game_state = world->GameState;
    if (!game_state) { all_initialized = false; return; };

    target = util::get_closest_to_center("Head");

    all_initialized = true;

    return;
}